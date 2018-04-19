package Validator::Group;
use Validator::Var;
use Carp;

use 5.006;
use strict;
use warnings;

=head1 NAME

Validator::Group - group of variables validators.

=cut

=head1 SYNOPSIS

    use Validator::Group;

    my $foo = Validator::Group->new();
    ...

=head1 SUBROUTINES/METHODS

=head2 new

Creates new validation group

=cut

sub new
{
    my $class = shift;
    my $self = bless {}, $class;
    return $self;
}


=head2 exists( $entry_id )

Returns true if Validator::Var instance specified by I<entry_id>
exists in the group.

=cut

sub exists
{
    my ($self, $vv_id) = @_;
    return defined $self->{$vv_id} ? 1 : 0;
}


=head2 validator( $vid )

Returns Validator::Var instance specified by I<vid>.
If variable's validator does not exist it will be created without any checker functions.

=cut

sub validator
{
    my ($self, $vid) = @_;
    $self->{$vid} = Validator::Var->new unless defined $self->{$vid};
    return $self->{$vid};
}


=head2 validate( $vars_HASHREF, $var_KEY=>$rule_ARRAYREF, $var_KEY=>$rule_ARRAYREF, ... )
=head2 validate( $vars_ARRAYREF, $rule_ARRAYREF, $rule_ARRAYREF, ... )

Validate group of variables according to specified rules.

    my $vgroup = Validator::Group->new;

    $vgroup->validator('Var0_Id')->checker(...)->checker(...)->...;
    $vgroup->validator('Var1_Id')->checker(...)->checker(...)->...;
    
    ...

    my %vars = $vgroup->validate({Var0=>$var0, Var1=>$var1, ..., VarN=>$varN, ...},
        'Var0'=>['Var0_Id', 'Var0 is bad'],
        'Var1'=>['Var1_Id', 'Var1 is bad'],
        ...,
        'VarN'=>[], # VarN skipped for validation
        ...);
        
    -or-
    
    my @vars = $vgroup->validate([$var0, $var1, ..., $varN, ...}],
        ['Var0_Id', 'Var0 is bad'],
        ['Var1_Id', 'Var1 is bad'],
        ...,
        [], # VarN skipped for validation
        ...);
        
Rule consist of two fields: Validator::Var instance identifier (entry_id)
and message carp'ed if variable is not valid.

=cut

sub validate
{
    my $self = shift;
    my $vars = shift;
    my $ok = 1;
    
    $vars or return 1; # no vars
    
    ref $vars or croak 'reference to HASH or ARRAY is expected';
    
    if( ref $vars eq 'HASH' ) {
        my %rules = @_;
        for my $k ( keys %{$vars} ) {
            next unless exists $rules{$k};
            next unless $rules{$k}; # empty array
            $vars->{$k} = $self->validator($rules{$k}->[0])->default unless defined $vars->{$k};
            
            $self->exists($rules{$k}->[0])
                or croak sprintf('no varibale validator found: \'%s\'', $rules{$k}->[0]);
            
            unless( $self->validator($rules{$k}->[0])->is_valid($vars->{$k}) ) {
                if( $rules{$k}->[1] ) {
                    carp $rules{$k}->[1];
                } else {
                    carp sprintf('varibale validator does not passed: \'%s\'', $rules{$k}->[0]);
                }
                $ok = 0;
            }
        }
    } elsif( ref $vars eq 'ARRAY' ) {
        my @rules = @_;
        
        scalar(@$vars) == scalar(@rules) or croak 'inappropriate number of rules';
        
        foreach my $i ( @{$vars} ) {
            next unless $rules[$i]; # empty array
            $vars->[$i] = $self->validator($rules[$i]->[0])->default unless defined $vars->[$i];
            
            $self->exists($rules[$i]->[0])
                or croak sprintf('no varibale validator found: \'%s\'', $rules[$i]->[0]);
            
            unless( $self->validator($rules[$i]->[0])->is_valid($vars->[$i]) ) {
                if( $rules[$i]->[1] ) {
                    carp $rules[$i]->[1];
                } else {
                    carp sprintf('varibale validator does not passed: \'%s\'', $rules[$i]->[0]);
                }
                $ok = 0;
            }
        }
        
    } else {
        croak 'reference to HASH or ARRAY is expected';
    }
    
    return $ok;
}


=head1 AUTHOR

Fedor Semenov, C<< <fedor.v.semenov at gmail.com> >>

=head1 BUGS

Please report any bugs or feature requests to C<bug-validator-var at rt.cpan.org>,
or through the web interface at L<http://rt.cpan.org/NoAuth/ReportBug.html?Queue=Validator-Var>.
I will be notified, and then you'll automatically be notified of progress
on your bug as I make changes.

=head1 SUPPORT

You can find documentation for this module with the perldoc command.

    perldoc Validator::Group


You can also look for information at:

=over 4

=item * RT: CPAN's request tracker (report bugs here)

L<http://rt.cpan.org/NoAuth/Bugs.html?Dist=Validator-Var>

=item * AnnoCPAN: Annotated CPAN documentation

L<http://annocpan.org/dist/Validator-Var>

=item * CPAN Ratings

L<http://cpanratings.perl.org/d/Validator-Var>

=item * Search CPAN

L<http://search.cpan.org/dist/Validator-Var/>

=back


=head1 ACKNOWLEDGEMENTS


=head1 LICENSE AND COPYRIGHT

Copyright 2011 Fedor Semenov.

This program is free software; you can redistribute it and/or modify it
under the terms of either: the GNU General Public License as published
by the Free Software Foundation; or the Artistic License.

See http://dev.perl.org/licenses/ for more information.


=cut

1; # End of Validator::Group
