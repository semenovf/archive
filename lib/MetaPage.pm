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
use strict;
use warnings;


sub new
{
    my $class = shift;
    return bless {}, (ref $class ? ref $class : $class);
}

sub render
{
    my ($self, $data) = @_;
    my $parser = MetaPage::Parser->new;
    $parser->media('html');
    MetaPage::HandlerFactory::Plain->handlersFor($parser);
    if( ref $data ) {
        $parser->parse_text($data);
    } else {
        -f $data or croak sprintf('File not found: %s', $data);
        $parser->parse_file($data);
    }
    $parser->release;
    return $parser->render;
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