#!env zsh

foreach this (cmake-utils) {
  git fetch $this-remote master
  git subtree pull --prefix 3rdparty/github/$this-remote $this-remote master --squash
}

exit 0
