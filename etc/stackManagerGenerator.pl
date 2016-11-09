#!env perl
use strict;
use POSIX qw/EXIT_SUCCESS EXIT_FAILURE/;
use Data::Dumper;
use Getopt::Long;

my $rulefile;
my $mapfile;
my $outfile;
my $prefix = '';
my $postfix = '_t';
my $errlogger = '';
my $tracelogger = '';
my $tracecond = '';
my $help = 0;
if (! GetOptions("rule=s"          => \$rulefile,
                 "map=s"           => \$mapfile,
                 "prefix=s"        => \$prefix,
                 "postfix=s"       => \$postfix,
                 "errlogger=s"     => \$errlogger,
                 "tracelogger=s"   => \$tracelogger,
                 "tracecond=s"     => \$tracecond,
                 "out=s"           => \$outfile,
                 "help!"           => \$help)) {
  exit(EXIT_FAILURE);
}

if ($help || ! $rulefile || ! $mapfile) {
  print <<USAGE;
  Usage: $0 [options]

  where options can be:

  --rule          rulefile       Load methods configuration from rulefile. Default: undef.
  --map           mapfile        Load type mappings from mapfile. Default: undef.
  --prefix        typeprefix     String preprended to every generated type. Default: "$prefix".
  --postfix       typepostfix    String appended to every generated type. Default: "$postfix".
  --out           outfile        Output. If not specified, it goes to STDOUT.
  --errlogger     method         If user context is logger aware, the name of method which accept the following arguments: method(void *userDatavp, char *fmts, ...). Eventual returns value does not matter. Default is none.
  --tracelogger   method         If user context is logger aware, the name of method which accept the following arguments: method(void *userDatavp, char *fmts, ...). Eventual returns value does not matter. Default is none.
  --tracecond     condition      A preprocessor condition to generate TRACE logs via the genericLogger. Default is none.
  --help                         This help

  Example:

  perl $0 --rule etc/ESLIF.rules --map etc/ESLIF.map --prefix marpaESLIF_bootstrap --errlogger marpaESLIF_bootstrap_logerrorv --tracelogger marpaESLIF_bootstrap_logtracev --tracecond "#ifndef MARPAESLIF_NTRACE"
USAGE
  exit(EXIT_SUCCESS);
}

my $out;
if ($outfile) {
  open($out, '>', $out) || die "$out, $!";
} else {
  $out = \*STDOUT;
}

my @rule = ();
my %type = ();
my %map = ();

loadRule($rulefile, \@rule, \%type);
loadMap($mapfile, \%map);
checkType(\%type, \%map);
generate(\%type, \%map);

die "No type detected" unless %type;

if ($outfile) {
  close($out, '>', $out) || warn "$out, $!";
}

sub loadRule {
  my ($file, $rulep, $typep) = @_;

  open(my $fh, '<', $file) || die "$file: $!";
  my $line = 0;
  while (chomp($_ = <$fh>)) {
    ++$line;
    #
    # We expect data to contain lines in the forms:
    # TYPE = METHOD(TYPE*)
    #
    # Perl'like comment lines are ignored
    # Space-only lines are ignored
    #
    # where a type is writen like this:
    # whatYouWant, or
    # whatYouWant [Quantifier]
    #
    # A Quantifier can be:
    # - '*' or nothing for an array of unknown size
    # - '+' for an array of size >= 1
    # - [N] for an array of N elements
    # - [N1..N2] for an array of minimum N1 to maximum N2 elements
    # - [N1..] for an array of minimum N1
    # - [..N2] for an array of maximum N2
    #
    # Please note that, ouf course, saying X[1] is different that X, as in C: the
    # first case is an array of size 1, the second case is X directly on the stack.
    #
    # Multiline is not supported: you have to write the prototype entirely.
    #
    my $input = $_;
    next if ($input =~ /^\s*#/);
    next if ($input =~ /^\s*$/);
    if ($input =~ m/^\s*(\w+)\s*=\s*(\w+)\s*\((.*)\)\s*;$/smg) {
      my ($outtype, $method, $intypes) = ($1, $2, $3 || '');
      $typep->{$outtype}++;
      $intypes =~ s/\s+//g;
      my @intypes = ();
      foreach my $item (split(/,/, $intypes)) {
        my ($intype, $minmax);

        die "$file.line: Failed to parse $input\n" if (! $item);
        if ($item =~ /^(\w+)$/) {
          ($intype, $minmax) = ($1, undef);
        } elsif ($item =~ /^(\w+)\+$/) {
          ($intype, $minmax) = ($1, [ 1, undef ]);
        } elsif ($item =~ /^(\w+)\*$/) {
          ($intype, $minmax) = ($1, [ 0, undef ]);
        } elsif ($item =~ /^(\w+)\[(\d+)\]$/) {
          ($intype, $minmax) = ($1, [ $2, $2 ]);
        } elsif ($item =~ /^(\w+)\[(\d+)\.\.(\d+)\]$/) {
          ($intype, $minmax) = ($1, [ $2, $3 ]);
        } elsif ($item =~ /^(\w+)\[(\d+)\.\.\]$/) {
          ($intype, $minmax) = ($1, [ $2, undef ]);
        } elsif ($item =~ /^(\w+)\[\.\.(\d+)\]$/) {
          ($intype, $minmax) = ($1, [ undef, $2 ]);
        } else {
          die "$file.$line: Problem with $item";
        }
        $typep->{$intype}++;
        push(@intypes, { type => $intype, minmax => $minmax });
      }
      push(@{$rulep}, { outtype => $outtype, method => $method, intypes => \@intypes });
    } else {
      die "$file.$line: not parsable";
    }
  }
  close($fh) || warn "$fh, $!";
}

sub loadMap {
  my ($file, $mapp) = @_;

  open(my $fh, '<', $file) || die"$file: $!";
  my $line = 0;
  while (chomp($_ = <$fh>)) {
    ++$line;
    #
    # We expect data to contain lines in the forms:
    # TYPE = STACKTYPE
    #
    # or
    #
    # TYPE = STACKTYPE{Ctype};
    #
    # Perl'like comment lines are ignored
    # Space-only lines are ignored
    #
    # where:
    # TYPE is what you want, should appear in the rule config file.
    # STACKTYPE is one supported by genericStack.h (INT, SHORT, LONG, PTR, ARRAY, etc...)
    # Ctype is the real C type behind.
    #
    # When Ctype is not specified, then the type is forced to GENERICSTACKITEMTYPE2TYPE_xxx where xxx is STACKTYPE,
    # unless if STACKTYPE is PTR, then it is forced to TYPE*.
    # STACKTYPE being equal to NA is translated to (void *)-1 in C terms
    # It is illegal to have more than occurence of TYPE per line
    #
    # Multiline is not supported: you have to write the prototype entirely.
    #
    my $input = $_;
    next if ($input =~ /^\s*#/);
    next if ($input =~ /^\s*$/);
    if ($input =~ m/^\s*(\w+)\s*=\s*(\w+)(?:\s*\{(.*)\s*\})?\s*;$/smg) {
      die "Type $1 already defined upper" if (exists($mapp->{$1}));
      $mapp->{$1} = {basictype => $2, stacktype => basictype2stacktype($2), ctype => $3 || basictype2ctype($2) };
    } else {
      die "$file.line: not parsable";
    }
  }
  close($fh) || warn "$fh, $!";
}

sub checkType {
  my ($typep, $mapp) = @_;

  foreach (sort keys %{$typep}) {
    die "Type $_ is not defined in the $mapfile" unless exists($mapp->{$_});
  }
}

sub generate {
  my ($typep, $mapp) = @_;

  #
  # First generate an enum for all the types
  #
  my $now = localtime;
  print  $out <<OUT;
/* This is a generated file as of $now */

#include <stdlib.h>        /* malloc, etc. */
#include <errno.h>         /* errno */
#include <string.h>        /* strerror(errno) */
#include <genericStack.h>  /* generic stack */

/* -------------------------------------------- */
/* Every item in the stack is explicitly tagged */
/* -------------------------------------------- */
typedef enum ${prefix}_itemType {
OUT
    foreach (sort keys %{$typep}) {
      printf $out uc(sprintf("\t%s_itemType_%s\n", ${prefix}, $_));
    }
    print $out <<OUT;
} ${prefix}_itemType_t;

/* ------------------------------------------------------------------------------- */
/* Every type mapped to a genericStack's PTR generates free and clone dependencies */
/* ------------------------------------------------------------------------------- */
OUT
  foreach (grep { $mapp->{$_}->{basictype} eq 'PTR' } sort keys %{$mapp}) {
    printf "static void %s(void *userDatavp, %s%s);\n", x2meth($_, 'freev'), $mapp->{$_}->{ctype}, $_;
  }
  print $out "\n";
  foreach (grep { $mapp->{$_}->{basictype} eq 'PTR' } sort keys %{$mapp}) {
    printf "static %s%s(void *userDatavp, %s%s);\n", $mapp->{$_}->{ctype}, x2meth($_, 'clonep'), $mapp->{$_}->{ctype}, $_;
  }
  #
  # Internal types and methods that the stack manager is using
  #
  print  $out <<OUT;

/* ----------------------------------------------- */
/* Stack internal type, constructor and destructor */
/* ----------------------------------------------- */
OUT
  printf $out "typedef %s %s;\n", stackManager(), stackManager_t();
  printf $out "static %s *%s(void *userDatavp);\n", stackManager_t(), stackManager_newp();
  printf $out "static void %s(void *userDatavp, %s *stackManagerp);\n", stackManager_freev(), stackManager_t();
  print  $out <<OUT;

/* ----------------------------------------------------------- */
/* Every item being tagged in the stack, methods to inspect it */
/* ----------------------------------------------------------- */
OUT
  printf $out "static ${prefix}_itemType_t %s(void *userDatavp, %s *stackManagerp, int i);\n", stackManager_i_gettypei(), stackManager_t();
  if ($tracelogger) {
    if ($tracecond) {
      print $out "$tracecond\n";
    }
    printf $out "static void %s(void *userDatavp, %s *stackManagerp, int i);\n", stackManager_i_tracev(), stackManager_t();
    if ($tracecond) {
      print $out "#endif /* $tracecond */\n";
    }
  }
  #
  # Finally, generate the method bodies
  #
  generateStackManagerConstrustor();
  generateStackManagerDestructor();
  generateStackManagerTrace($typep);
}

sub basictype2ctype {
  my ($basictype) = @_;
  our %BASICTYPE2CTYPE = (
                          'CHAR' => 'char',
                          'SHORT' => 'short',
                          'INT' => 'int',
                          'LONG' => 'long',
                          'FLOAT' => 'float',
                          'DOUBLE' => 'double',
                          'PTR' => 'void *',
                          'LONG_LONG' => 'long long',
                          '_BOOL' => '_Bool',
                          'FLOAT_COMPLEX' => 'float _Complex',
                          'DOUBLE_COMPLEX' => 'double _Complex',
                          'LONG_DOUBLE_COMPLEX' => 'long double _Complex',
                          'NA' => 'void'
                         );
  return $BASICTYPE2CTYPE{$basictype} || die "Unsupported type $basictype when converting to a C type, should be one of: " . join(', ', sort keys %BASICTYPE2CTYPE);
}

sub basictype2stacktype {
  my ($basictype) = @_;
  our %BASICTYPE2STACKTYPE = (
                              'CHAR' => 'GENERICSTACKITEMTYPE2TYPE_CHAR',
                              'SHORT' => 'GENERICSTACKITEMTYPE2TYPE_SHORT',
                              'INT' => 'GENERICSTACKITEMTYPE2TYPE_INT',
                              'LONG' => 'GENERICSTACKITEMTYPE2TYPE_LONG',
                              'FLOAT' => 'GENERICSTACKITEMTYPE2TYPE_FLOAT',
                              'DOUBLE' => 'GENERICSTACKITEMTYPE2TYPE_DOUBLE',
                              'PTR' => 'GENERICSTACKITEMTYPE2TYPE_PTR',
                              'LONG_LONG' => 'GENERICSTACKITEMTYPE2TYPE_LONG_LONG',
                              '_BOOL' => 'GENERICSTACKITEMTYPE2TYPE__BOOL',
                              'FLOAT_COMPLEX' => 'GENERICSTACKITEMTYPE2TYPE_FLOAT__COMPLEX',
                              'DOUBLE_COMPLEX' => 'GENERICSTACKITEMTYPE2TYPE_DOUBLE__COMPLEX',
                              'LONG_DOUBLE_COMPLEX' => 'GENERICSTACKITEMTYPE2TYPE_LONG_DOUBLE__COMPLEX',
                              'NA' => 'GENERICSTACKITEMTYPE2TYPE_NA'
                             );
  return $BASICTYPE2STACKTYPE{$basictype} || die "Unsupported type $basictype when converting to a stack type, should be one of: " . join(', ', sort keys %BASICTYPE2STACKTYPE);
}
sub x2meth { return sprintf("${prefix}_%s_%s", @_) }
sub stackManager {
  return "struct stackManager {
  genericStack_t *dataStackp;
  genericStack_t *typeStackp;
}";
}
sub stackManager_t { return "${prefix}_stackManager_t" }
sub stackManager_newp { return "${prefix}_stackManager_newp" }
sub stackManager_freev { return "${prefix}_stackManager_freev" }
sub stackManager_i_tracev { return "${prefix}_stackManager_i_tracev" }
sub stackManager_i_gettypei { return "${prefix}_stackManager_i_gettypei" }
sub stackManager_error {
  my $indent = shift;
  if ($errlogger) {
    printf $out "%*s$errlogger(%s, %s);\n", $indent, " ", "userDatavp", join(', ', @_);
  }
}
sub stackManager_trace {
  my $indent = shift;
  if ($tracelogger) {
    if ($tracecond) {
      print $out "$tracecond\n";
    }
    printf $out "%*s$tracelogger(%s, %s);\n", $indent, " ", "userDatavp", join(', ', @_);
    if ($tracecond) {
      print $out "#endif /* $tracecond */\n";
    }
  }
}

sub generateStackManagerConstrustor {
  print  $out <<OUT;

/* ------------------------- */
/* Stack Manager constructor */
/* ------------------------- */
OUT
  printf $out "static %s *%s(void *userDatavp) {\n", stackManager_t(), stackManager_newp();
  printf $out "  %s *stackManagerp;\n", stackManager_t();
  print  $out "\n";
  printf $out "  stackManagerp = (%s *) malloc(sizeof(%s));\n", stackManager_t(), stackManager_t();
  print  $out "  if (stackManagerp == NULL) {\n";
  stackManager_error(4, '"malloc failure, %s"', "strerror(errno)");
  print  $out "    goto err;\n";
  print  $out "  }\n";
  print  $out "\n";
  print  $out "  stackManagerp->dataStackp == NULL;\n";
  print  $out "  stackManagerp->typeStackp == NULL;\n";
  print  $out "\n";
  print  $out "  GENERICSTACK_INIT(stackManagerp->dataStackp)\n";
  print  $out "  if (GENERICSTACK_ERROR(stackManagerp->dataStackp)) {\n";
  stackManager_error(6, '"stackManagerp->dataStackp initializationfailure, %s"', "strerror(errno)");
  print  $out "    goto err;\n";
  print  $out "  }\n";
  print  $out "\n";
  print  $out "  GENERICSTACK_INIT(stackManagerp->typeStackp)\n";
  print  $out "  if (GENERICSTACK_ERROR(stackManagerp->typeStackp)) {\n";
  stackManager_error(6, '"stackManagerp->typeStackp initializationfailure, %s"', "strerror(errno)");
  print  $out "    goto err;\n";
  print  $out "  }\n";
  print  $out "  goto done;\n";
  print  $out "\n";
  print  $out " err:\n";
  printf $out "  %s(userDatavp, stackManagerp);\n", stackManager_freev();
  print  $out "  stackManagerp = NULL;\n";
  print  $out "\n";
  print  $out " done:\n";
  print  $out "  return stackManagerp;\n";
  print  $out "}\n";
}

sub generateStackManagerDestructor {
  print  $out <<OUT;

/* ------------------------- */
/* Stack Manager destructor */
/* ------------------------- */
OUT
  printf $out "static void %s(void *userDatavp, %s *stackManagerp) {\n", stackManager_freev(), stackManager_t();
  print  $out <<OUT;
  int i;
  int usedi;

  if (stackManagerp != NULL) {
    if (stackManagerp->dataStackp != NULL) {
      usedi = GENERICSTACK_USED(stackManagerp->dataStackp)
      for (i = 0; i < usedi; i++) {
      }
    }
    if (stackManagerp->typeStackp != NULL) {
      GENERICSTACK_FREE(stackManagerp->typeStackp); /* It contain only enums, nothing on the heap */
    }
    free(stackManagerp);
  }
}
OUT
}

sub generateStackManagerTrace {
  my ($typep) = @_;
  if ($tracelogger) {
    print $out "\n";
    if ($tracecond) {
      print $out "$tracecond\n";
    }
  print  $out <<OUT;
/* ------------------- */
/* Stack Manager trace */
/* ------------------- */
OUT
    printf $out "static void %s(void *userDatavp, %s *stackManagerp, int i) {\n", stackManager_i_tracev(), stackManager_t();
    print  $out "  ${prefix}_itemType_t itemType;\n";
    print  $out "  genericStack_t *typeStackp;\n";
    print  $out <<OUT;

  if (stackManagerp != NULL) {
    typeStackp = stackManagerp->typeStackp;
    if (typeStackp != NULL) {
      if (GENERICSTACK_IS_INT(typeStackp, i)) {
        itemType = (${prefix}_itemType_t) GENERICSTACK_GET_INT(typeStackp, i);
        switch (itemType) {
OUT
    foreach (sort keys %{$typep}) {
      my $basecase = uc($_);
      my $case = uc(sprintf("%s_itemType_%s", ${prefix}, $_));
      printf $out "        case $case:\n";
      print  $out "          $tracelogger(userDavavp, \"stack[%d] type is %s\", i, \"$basecase\");\n";
      printf $out "          break;\n";
    }
    printf $out "        default:\n";
      print  $out "          $tracelogger(userDavavp, \"stack[%d] type is unknown\", i);\n";
    printf $out "          break;\n";
    print  $out <<OUT;
        }
      }
    }
  }
OUT
    printf $out "}\n";
    if ($tracecond) {
      print $out "#endif /* $tracecond */\n";
    }
  }
}

exit(EXIT_SUCCESS);
