package Class::Accessor::Validated;
use base 'Class::Accessor';
use strict;
our $VERSION = '0.01';

sub mk_accessors
{
    my($self, %specs) = @_;
    $self->_mk_accessors('rw', \%specs);
}

sub mk_wo_accessors
{
    my($self, %specs) = @_;
    $self->_mk_accessors('wo', \%specs);
}


sub mk_ro_accessors
{
    my($self, %specs) = @_;
    $self->_mk_accessors('ro', \%specs);
}


{
    no strict 'refs';

    sub import {
        my ($class, @what) = @_;
        my $caller = caller;
        for (@what) {
            if (/^(?:antlers|moose-?like)$/i) {
                *{"${caller}::has"} = sub {
                    my ($f, %args) = @_;
                    $caller->_mk_accessors(($args{is}||"rw"), {$f =>$args{validator}||sub{1}});
                };
                *{"${caller}::extends"} = sub {
                    @{"${caller}::ISA"} = @_;
                    unless (grep $_->can("_mk_accessors"), @_) {
                        push @{"${caller}::ISA"}, $class;
                    }
                };
                # we'll use their @ISA as a default, in case it happens to be
                # set already
                &{"${caller}::extends"}(@{"${caller}::ISA"});
            }
        }
    }

    sub _mk_accessors {
        my($self, $access, $specs) = @_; # spec := { field => validator_function, ... }
        my $class = ref $self || $self;
        my $ra = $access eq 'rw' || $access eq 'ro';
        my $wa = $access eq 'rw' || $access eq 'wo';

        foreach my $field ( keys %$specs ) { # modified
            my $accessor_name = $self->accessor_name_for($field);
            my $mutator_name = $self->mutator_name_for($field);
            
            if( $accessor_name eq 'DESTROY' or $mutator_name eq 'DESTROY' ) {
                $self->_carp("Having a data accessor named DESTROY  in '$class' is unwise.");
            }
            
            if ($accessor_name eq $mutator_name) {
                my $accessor;
                if ($ra && $wa) {
                    $accessor = $self->make_accessor($field, $specs->{$field});
                } elsif ($ra) {
                    $accessor = $self->make_ro_accessor($field);
                } else {
                    $accessor = $self->make_wo_accessor($field, $specs->{$field});
                }
                my $fullname = "${class}::$accessor_name";
                my $subnamed = 0;
                unless (defined &{$fullname}) {
                    subname($fullname, $accessor) if defined &subname;
                    $subnamed = 1;
                    *{$fullname} = $accessor;
                }
                if ($accessor_name eq $field) {
                    # the old behaviour
                    my $alias = "${class}::_${field}_accessor";
                    subname($alias, $accessor) if defined &subname and not $subnamed;
                    *{$alias} = $accessor unless defined &{$alias};
                }
            } else {
                my $fullaccname = "${class}::$accessor_name";
                my $fullmutname = "${class}::$mutator_name";
                if ($ra and not defined &{$fullaccname}) {
                    my $accessor = $self->make_ro_accessor($field);
                    subname($fullaccname, $accessor) if defined &subname;
                    *{$fullaccname} = $accessor;
                }
                if ($wa and not defined &{$fullmutname}) {
                    my $mutator = $self->make_wo_accessor($field, $specs->{$field});
                    subname($fullmutname, $mutator) if defined &subname;
                    *{$fullmutname} = $mutator;
                }
            }
        }
    }
}

sub make_accessor {
    my ($class, $field, $func) = @_;

    return sub {
        my $self = shift;

        if(@_) {
            $func->(@_) or $self->_croak('Invalid value');
            return $self->set($field, @_);
        } else {
            return $self->get($field);
        }
    };
}

sub make_wo_accessor {
    my($class, $field, $func) = @_;

    return sub {
        my $self = shift;

        unless (@_) {
            my $caller = caller;
            $self->_croak("'$caller' cannot access the value of '$field' on objects of class '$class'");
        } else {
            $func->(@_) or $self->_croak('Invalid value');
            return $self->set($field, @_);
        }
    };
}

1;

__END__

=head1 NAME

  Class::Accessor::Validated - Automated accessor generation with validation support for mutators

=head1 SYNOPSIS

  package Foo;
  use base qw(Class::Accessor::Validated);
  Foo->follow_best_practice;
  Foo->mk_accessors(
    'integer'  => sub { $ =~ /^\d+$/ },
    'float'    => sub { $ =~ /^\d*\.\d+$/ },
    'id'       => sub { length($_) == 64 && $ =~ /[a-zA-Z0-9]/ }
  );
  
  package Bar;
  use base qw(Class::Accessor::Validated);
  Bar->mk_accessors(
    'ein'  => sub { 1 },
    'zwei' => sub { 1 },
    'drei' => sub { 1 }
  );

  package Baz;
  use base qw(Class::Accessor);
  Baz->mk_accessors( qw(ein, zwei drei) );

  # Bar and Baz are equivalent

=head1 DESCRIPTION

This module automagically generates accessors/mutators for your class
with validation support for mutators.

=head1 AUTHOR

Copyright 2011 Fedor Semenov <fedor.v.semenov@gmail.com>

This program is free software; you can redistribute it and/or modify it under
the same terms as Class::Accessor module.
Fully derived from the code of Class::Accessor, except a bit of code linked to validation.

=head1 CONSTRUCTOR

See L<Class::Accessor>.

=head1 MAKING ACCESSORS

=head2 mk_accessors

    __PACKAGE__->mk_accessors(%rw_specs);

This creates accessor/mutator methods for each named field given by
%rw_specs keys.

    # Generates foo(), _foo_accessor(), bar() and _bar_accessor().
    __PACKAGE__->mk_accessors(
        'foo' => sub { is_it_really_foo() },
        'bar' => sub { is_it_really_bar() } );

=head2 mk_ro_accessors

  __PACKAGE__->mk_wo_accessors(%write_only_specs);

Generates read-only accessors.

=head2 mk_wo_accessors

  __PACKAGE__->mk_wo_accessors(%write_only_specs);

Generates write-only accessors (ie. mutators).

=head1 DETAILS

=head2 make_accessor

    $accessor = __PACKAGE__->make_accessor($field, $func);

Generates a subroutine reference which acts as an accessor for the given
$field with validator set to $func.

=head2 make_wo_accessor

    $read_only_accessor = __PACKAGE__->make_wo_accessor($field, $func);

Generates a subroutine refrence which acts as a write-only accessor
(mutator) for the given $field with validator set to $func.

=head1 SEE ALSO

See L<Class::Accessor>, L<Class::Accessor::Fast> and L<Class::Accessor::Faster>.
Also L<Validator::Var>.

=cut
