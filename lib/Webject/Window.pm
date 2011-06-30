package Webject::Window;
use base 'Webject';

use 5.006;
use strict;
use warnings;

my $XHTML_1_0_Strict       = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">';
my $XHTML_1_0_Transitional = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">';
my $XHTML_1_0_Frameset     = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">';
my $XHTML_1_0_Mobile       = '<!DOCTYPE html PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.0//EN" "http://www.wapforum.org/DTD/xhtml-mobile10.dtd">';
my $XHTML_1_1              = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">';

my @rw_accessors = qw(
    _doctype
    title
);
__PACKAGE__->mk_accessors(@rw_accessors);

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->title('&lt;untitled&gt;');
    return $self;
}


sub render {
    my $self = $_[0];
    $self->_doctype($XHTML_1_0_Transitional);
    $self->render_file(*DATA); }

1;

__DATA__

<!--html{

<% $self->_doctype %>
<html>
<head>
<title><% $self->title %></title>
</head>
<body>

<% $self->render_children %>

</body>
</html>

}html-->