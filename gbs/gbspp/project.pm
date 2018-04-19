package gbspp::project;
use base qw(Class::Accessor::Validated);
use Carp;
use gbspp::type::string;
use strict;
use warnings;

# Global constants
#
use constant ARCH_X86     => 'x86';
use constant ARCH_X86_64  => 'x86_64';

our @EXPORT = ('project', 'ARCH_X86', 'ARCH_X86_64');

use constant FIELD_NAME => 'name';
use constant FIELD_ARCH => 'architecture';

#gbspp::project->follow_best_practice;
gbspp::project->mk_accessors(
      FIELD_NAME => sub { gbspp::type::string->can_assign($_); }
    , FIELD_ARCH => sub { $_ eq ARCH_X86 or $_ eq ARCH_X86_64; }
);

my %_projects = (); # project_name0 => object0, project_name1 => object1
my @_projects = (); # project_name0, project_name1, ...
my $_current_project = undef;

sub project
{
    my $name = shift or croak("Name for project must be specified");
    
    if (exists($_projects{$name})) {
        $_current_project = $_projects{$name};
    } else {
        $_current_project = __PACKAGE__->new(FIELD_NAME=>$name);
        $_projects{$name} = $_current_project;
        push(@_projects, $name);
    }
}

1;
