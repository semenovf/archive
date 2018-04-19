package gbspp::entity;
use base 'Exporter';
use gbspp::type::string;
use strict;
use warnings;

#
# Entity types
#
my $ENTITY_WORKSPACE => 'workspace';
my $ENTITY_SOLUTION  => 'solution';
my $ENTITY_PROJECT   => 'project';

my %_current_entities = (
      $ENTITY_WORKSPACE => undef
    , $ENTITY_SOLUTION  => undef
    , $ENTITY_PROJECT   => undef
);

my $FIELD_NAME = 'name';

sub new
{
    my $class = shift;
    my $type  = shift or die "Entity type must be specified";

    die "Bad entity type: $type" unless exists($_current_entities{$type});
    
    my $name  = shift or die "Name for '$type' must be specified";
    
    my $self = bless {
          $FIELD_NAME => gbspp::type::string->new($name)
    }, $class;

    $_current_entities{$type} = $self;

    return $self;
}

sub name
{
    return $_[0]->{$FIELD_NAME};
}

#
# Universal accessor/mutator
#
# Usage:
#   1. _am($type, $class, $field)
#       Get value with $field for current entity
#       Return value
#   
#   2. _am($type, $class, $field, $value)
#       Set value with $field for current entity
#       Return entity reference
#
#   3. _am($type, $class, $entity, $field)
#       Get value with $field for $entity
#       Return value
#
#   4. _am($type, $class, $entity, $field, $value)
#       Set value with $field for $entity
#       Return entity reference
#
sub _am
{
    my $type  = shift or die "Entity type must be specified";
    my $class = shift or die "Class type must be specified";
    die "Bad entity type: $type" unless exists($_current_entities{$type});

    my $_current_entity = $_current_entities{$type};
    
    my $entity = (scalar(@_) > 1 and ref($_[0]) eq $class) ? $_[0] : $_current_entity;
    my $name   = $entity->{$FIELD_NAME}->value;
    
    my $field = undef;
    $field = $_[0] if scalar(@_) == 1;                            # case 1
    $field = $_[0] if (scalar(@_) == 2) and ref($_[0]) ne $class; # case 2
    $field = $_[1] if (scalar(@_) == 2) and ref($_[0]) eq $class; # case 3
    $field = $_[1] if (scalar(@_) == 3) and ref($_[0]) eq $class; # case 4
    
    die "Bad call of universal accessor/mutator" unless defined($field);
    
    my $value = undef;
    $value = $_[1] if (scalar(@_) == 2) and ref($_[0]) ne $class; # case 2
    $value = $_[2] if (scalar(@_) == 3) and ref($_[0]) eq $class; # case 4
    
    die "Bad property '$field' for $type '$name'" unless defined $entity->{$field};
    
    # Mutator
    #
    if (defined($value)) {
        $entity->{$field}->assign($value);
        return $entity;
    }
    
    return $entity->{$field}->value;
}

#
# Parse accessor/mutator signature
#
#  _am_sig($type, $class, $caller_name, $field_name)
#  _am_sig($type, $class, $caller_name, $field_name, $value)
#  _am_sig($type, $class, $caller_name, $field_name, $entity)
#  _am_sig($type, $class, $caller_name, $field_name, $entity, $value)
#
# Return array ($entity, $field_name, $value)
#
sub _am_sig
{
    my $type  = shift or die "Entity type must be specified";
    my $class = shift or die "Class type must be specified";
    die "Bad entity type: $type" unless exists($_current_entities{$type});

    my $caller_name = shift or die "Caller name must be specified";
    my $field_name  = shift or die "Field name must be specified";
    my $entity      = $_current_entities{$type};
    my $value       = undef;
    
    return ($_current_entity, undef) if scalar(@_) == 0;
    return (ref($_[0]), undef) if scalar(@_) == 1 and ref($_[0]) eq __PACKAGE__;
    
    die "Bad call signature for $caller_name";
    
    return ($entity, $field_name, $value);
}


1;
