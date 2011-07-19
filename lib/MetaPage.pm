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
    return $self;
}

sub render
{
    my ($self, $data, $vars) = @_;
    my $parser = MetaPage::Parser->new(ErrorContext=>4);
    $parser->media('html');
    $parser->metapage($self);
    $parser->root(1);
    MetaPage::HandlerFactory::Plain->handlersFor($parser);

    $self->vars->set_vars($vars) if $vars && ref $vars && ref $vars eq 'HASH';
    if( ref $data ) {
        $parser->parse_text($data);
    } else {
        -f $data or croak sprintf('%s: File not found', $data);
        $parser->parse_file($data);
    }
   
    local @ARGV;
    $ARGV[0] = $self->vars;
    my $il_code = $parser->render_il_code;
    
    #print $il_code;

    my $warning;
    local $SIG{__WARN__} = sub { $warning = $_[0] };
    
    # execute the code
    my $content = eval $il_code;
    my $ex = $@ || $warning;
    if( $ex ) {
        my @lines  = split ("\n", $il_code);
        my $line_counter = 0;
        my $sd  = 0; # maximum number of significant digits
        while( scalar @lines > $sd ** 10 ) { $sd++; }
        croak "$ex\n", join("\n", map { sprintf('%0' . $sd . 'd: %s', ++$line_counter, $_) } @lines );
    }
    
    $parser->release;
    $parser = undef;
    return $_post_render->($content);
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