use strict;
use warnings FATAL => 'all';

package MarpaX::ESLIF::JSON::Decoder;
use MarpaX::ESLIF::Registry;
use MarpaX::ESLIF::JSON::Decoder::RecognizerInterface;
use parent qw/MarpaX::ESLIF::Grammar/;

my $CLONABLE = 1;

# ABSTRACT: ESLIF's JSON decoder interface

# AUTHORITY

# VERSION

=head1 DESCRIPTION

This is JSON's strict and relax decoder writen directly in L<MarpaX::ESLIF> library.

There are two JSON modes:

=over

=item Strict

Encoder and decoder are strict, as per L<ECMA-404 The JSON Data Interchange Standard|https://www.json.org>.

=item Relax

This is strict grammar extended with:

=over

=item Unlimited commas

=item Trailing separator

=item Perl style comment

=item C++ style comment

=item Infinity

=item NaN

=item Unicode's control characters (range C<[\x00-\x1F]>).

=back

=back

=cut

=head1 METHODS

=head2 MarpaX::ESLIF::JSON::Decoder->new($eslif[, $strict])

   my $eslifJSONDecoder = MarpaX::ESLIF::JSON::Decoder->new($eslif);

Returns a JSON grammar instance, noted C<$eslifJSONDecoder> later. Parameters are:

=over

=item C<$eslif>

MarpaX::ESLIF object instance. Required.

=item C<$strict>

A true value means strict JSON, else relax JSON. Default is a true value.

=back

=cut

#
# Tiny wrapper on MarpaX::ESLIF::JSON::Decoder->new, that is using the instance as void *.
# Could have been writen in the XS itself, but I feel it is more comprehensible like
# this.
#
sub _allocate {
    my ($class, $eslif, @rest) = @_;

    return MarpaX::ESLIF::Engine::JSON::Decoder::allocate->($eslif->{engine}, @rest)
    
}

sub _dispose {
    my ($class) = shift;

    return MarpaX::ESLIF::Grammar::Engine->dispose(@_)
}

sub new {
    my ($class, $eslif, $strict) = @_;
    
    return MarpaX::ESLIF::Registry::new($class, $CLONABLE, undef, \&_allocate, \&_dispose, $eslif, $strict // 1)
}

=head2 $eslifJSONDecoder->decode($string, %options)

   my $value = $eslifJSONDecoder->decode($string);

Returns a value containing decoded C<$string>. In relax mode, special floating point are supported:

=over

=item C<+Infinity>

Positive infinity, either as a native floating point number if the underlying system supports that, or as a C<Math::BigInt->binf()> instance.

=item C<-Infinity>

Negative infinity, either as a native floating point number if the underlying system supports that, or as a C<Math::BigInt->binf('-')> instance.

=item C<NaN>

Not-a-Number, either as a native floating point number if the underlying system supports that, or as a C<Math::BigInt->bnan()> instance.

=back

Supported options are:

=over

=item encoding

Input encoding. Can be C<undef>.

=item disallowDupkeys

A true value will disallow duplicate keys. Default is a false value.

=item maxDepth

Maximum depth. Default is 0, meaning no limit.

=item noReplacementCharacter

A true value will disallow UTF-8 replacement character for invalid UTF-16 surrogates. Default is a false value.

=back

=cut

sub decode {
    my ($self, $string, %options) = @_;

    my $recognizerInterface = MarpaX::ESLIF::JSON::Decoder::RecognizerInterface->new($string, $options{encoding});
    return MarpaX::ESLIF::JSON::Decoder::_decode($self, $recognizerInterface, $options{disallowDupkeys}, $options{maxDepth}, $options{noReplacementCharacter})
}

=head1 NOTES

Formally, the JSON implementation is only a grammar coded directly in the ESLIF library, therefore this module inherits from L<MarpaX::ESLIF::Grammar>.

=cut

sub DESTROY {
    goto &MarpaX::ESLIF::Registry::DESTROY
}

1;
