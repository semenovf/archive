package gbs::solution;
use Carp;
use strict;
use warnings;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = ('solution');

#my %_solutions = (); # solution_name0 => object0, solution_name1 => object1
#my @_solutions = (); # solution_name0, solution_name1, ...
#my $_current_solution = undef;
#
sub solution
{
    #my $name = shift or croak("Name for solution must be specified");
    #
    #if (exists($_solutions{$name})) {
    #    $_current_solution = $_solutions{$name};
    #} else {
    #    $_current_solution = __PACKAGE__->new(FIELD_NAME=>$name);
    #    $_solutions{$name} = $_current_solution;
    #    push(@_solutions, $name);
    #}
}

1;
