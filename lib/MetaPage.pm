#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#

package MetaPage;
use Carp;
use MetaPage::Parser;
use MetaPage::HandlerFactory::Plain;
use MetaPage::Var;
use base 'Class::Accessor';
use strict;
use warnings;

my $_post_render = sub {$_[0]};

__PACKAGE__->mk_accessors(qw(
    vars
    dump_code
));


#
#@TODO: need modification for implement dynamic post renderer
# by definition in form:
# use MetaPage qw(Text::Simplify)
#
sub import
{
    require Text::Simplify;
    $_post_render = \&Text::Simplify::simplify;
}

sub new
{
    my $class = shift;
    my $self = bless $class->SUPER::new, (ref $class ? ref $class : $class);
    $self->vars( MetaPage::Var->new );
    $self->dump_code(undef);
    return $self;
}

sub render
{
    my ($self, $data, $vars) = @_;
    my $parser = MetaPage::Parser->new(ErrorContext=>4);
    $parser->media('html');
    $parser->metapage($self);
    MetaPage::HandlerFactory::Plain->handlersFor($parser);

    $self->vars->set_vars($vars) if $vars && ref $vars && ref $vars eq 'HASH';
    
    if( ref $data ) {
        $parser->parse_text($data);
    } else {
        -f $data or croak sprintf('File not found: %s', $data);
        $parser->parse_file($data);
    }
    $parser->release;
    return $_post_render->($parser->render);
}

1;

__END__

=head1

    use MetaPage;
    
    my $mp = MetaPage->new;

    my $content = $mp->render('path/to/template');
    -or-
    my $content = $mp->render(\$text);

=cut