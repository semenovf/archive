package gbspp::workspace;
use base qw(Class::Accessor::Validated);
use Carp;
use gbspp::type::string;
use strict;
use warnings;

our @EXPORT = ('workspace');

use constant FIELD_NAME => 'name';

#gbspp::project->follow_best_practice;
gbspp::project->mk_accessors(
      FIELD_NAME => sub { gbspp::type::string->can_assign($_); }
);

my %_workspaces = (); # workspace_name0 => object0, workspace_name1 => object1
my @_workspaces = (); # workspace_name0, workspace_name1, ...
my $_current_workspace = undef;

sub workspace
{
    my $name = shift or croak("Name for workspace must be specified");
    
    if (exists($_workspaces{$name})) {
        $_current_workspace = $_workspaces{$name};
    } else {
        $_current_workspace = __PACKAGE__->new(FIELD_NAME=>$name);
        $_workspaces{$name} = $_current_workspace;
        push(@_workspaces, $name);
    }
}

1;
