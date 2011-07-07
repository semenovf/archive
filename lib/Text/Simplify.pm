package Text::Simplify;

use strict;
use warnings;

=head1 NAME

Text::Simplify - returns a string that has whitespace removed from the start and the end,
and that has each sequence of internal whitespace replaced with a single space.

=head1 VERSION

version 0.01

=cut

our $VERSION = '0.01';

=head1 SYNOPSIS

    Text::Simplify;

    $text = "\tThis is a test    string   \n for simplifying \t\n data\n";
    $data = simplify $text;
    # now $data contains "This is a test string for simplifying data" and $text is unchanged
    
    # or:
    simplify $text; # work in-place
    # $text now contains "This is a test string for simplifying data"

    @lines = <STDIN>;
    simplify @lines; # simplify all lines

    # Alternatively:
    @lines = simplify <STDIN>;

    # Or even:
    while (<STDIN>) {
        simplify; # Change $_ in place
        # ...
    }

=head1 DESCRIPTION

This module provides function for simplifying a string by removing whitespaces
from the start and the end and replacing each sequence of internal whitespaces
with a single space.
This module is intended to be an addition to Matt Lawrence' L<Text::Trim>.

=head1 EXPORTS

Function simplify exported by default.

=cut

use Exporter;

our @ISA = qw( Exporter );
our @EXPORT = qw( simplify );

=head1 CONTEXT HANDLING

=head2 void context

Functions called in void context change their arguments in-place

    simplify(@strings); # All strings in @strings are simplified in-place

    simplify($text); # simplify on $text

    simplify; # simplify on $_

No changes are made to arguments in non-void contexts.

=head2 list context

Values passed in are changed and returned without affecting the originals.

    @result = simplify(@strings); # @strings is unchanged

    @result = simplify; # @result contains simplified $_

    ($result) = simplify(@strings); # like $result = simplify($strings[0]);

=head2 scalar context

As list context but multiple arguments are stringified before being returned.
Single arguments are unaffected.  This means that under these circumstances, the
value of $" ($LIST_SEPARATOR) is used to join the values. If you don't want
this, make sure you only use single arguments when calling in scalar context.

    @strings = ("\thello  \n", "\t   there \n");
    $text = simplify(@strings);
    # $text = "hello there"

    local $" = ', ';
    $text = simplify(@strings);
    # Now $text = "hello, there"

    $text = simplify;
    # $text = simplified $_

=head2 Undefined values

If any of the functions are called with undefined values, the behaviour is in
general to pass them through unchanged. When stringifying a list (calling in
scalar context with multiple arguments) undefined elements are excluded, but
if all elements are undefined then the return value is also undefined.

    $foo = simplify(undef);        # $foo is undefined
    $foo = simplify(undef, undef); # $foo is undefined
    @foo = simplify(undef, undef); # @foo contains 2 undefined values
    simplify(@foo)                 # @foo still contains 2 undefined values
    $foo = simplify('', undef);    # $foo is ''

=head1 FUNCTIONS

=head2 simplify

Removes leading and trailing whitespaces and replaces each sequence of internal
whitespaces with a single space.

=cut

sub simplify {
    @_ = @_ ? @_ : $_ if defined wantarray;

    for (@_ ? @_ : $_) { next unless defined; s/\A\s+//; s/\s+\z//; s/\s+/ /; }

    return @_ if wantarray || !defined wantarray;

    if (my @def = grep defined, @_) { return "@def" } else { return }
}


1;

__END__


=head1 SEE ALSO

Matt Lawrence' L<Text::Trim>.

=head1 AUTHOR

Fedor Semenov, C<< <fedor.v.semenov at gmail.com> >>

=head1 ACKNOWLEDGEMENTS

=cut

