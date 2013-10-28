#!/usr/bin/perl -w
use Params::Validate qw(:all);
use Data::Dumper;
use Test::More;
use strict;

plan tests => 6;

eval { ok(foo(), 'foo()'); };
eval { ok(foo(one=>123), 'foo(one=>123)'); };
eval { ok(foo(two=>123), 'foo(two=>123)'); };
eval { ok(foo(three=>123), 'foo(three=>123)'); };
eval { ok(foo(one=>123, two=>[], three=>{}), 'foo(one=>123, two=>[], three=>{})'); };
ok(bar(123, -foo=>'foo', -bar=>'bar'), 'bar(123, -foo=>\'foo\', -bar=>\'bar\')');

done_testing();

sub foo
{
    my %p = validate( @_, {
        one   => { type=>SCALAR },
        two   => { type=>ARRAYREF },
        three => { type=>HASHREF }
    });
}


sub bar
{
    my $self;
    ($self, @_) = validate_pos( @_, { type=>SCALAR }, (0) x (@_ - 1) );
    my %a = validate( @_, {
        -foo=>{ type=>SCALAR },
        -bar=>{ type=>SCALAR }
    });
    print Dumper(%a);
}

0;

__END__

=head1 Heading text head1

=cut

=head2 Heading text head2

=cut

=head3 Heading text head3

=cut

=head4 Heading text head4

=cut


=over

=item Item1

Item1 inner text

=cut

=item Item2

I<Italic>
B<Bold>
C<Code>
L<Hyperlink>
10 E<lt> 20 E<gt> 15
F<filename>
S<text contains non-breaking spaces>
X<an index entry>

=cut

##
=item Item3

Item3 inner text

=cut

=item Item4

Item4 inner text

=cut

=back
