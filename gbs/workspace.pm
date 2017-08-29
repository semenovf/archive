package gbs::workspace;
use Carp;
use strict;
use warnings;
use gbs::context;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = ('workspace');

#my %_workspaces = (); # workspace_name0 => object0, workspace_name1 => object1
#my @_workspaces = (); # workspace_name0, workspace_name1, ...
#my $_current_workspace = undef;

sub workspace
{
    carp "workspace: any arguments are ignored" if scalar(@_) > 0;
    
    #my $name = shift or croak("Name for workspace must be specified");
    #
    #if (exists($_workspaces{$name})) {
    #    $_current_workspace = $_workspaces{$name};
    #} else {
    #    $_current_workspace = __PACKAGE__->new(FIELD_NAME=>$name);
    #    $_workspaces{$name} = $_current_workspace;
    #    push(@_workspaces, $name);
    #}
}

1;
