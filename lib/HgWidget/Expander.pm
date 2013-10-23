package HgWidget::Expander;

my $id_counter = 0;

sub new
{
    my $class = shift or die;
    my $id = shift // "jqxExpander_$id_counter";
    my $self = {
        id => $id
    };
    return bless $self, $class;
}

sub default_header_id
{
    my $self = shift;
    return $self->{id} . 'Header';
}

sub default_content_id
{
    my $self = shift;
    return $self->{id} . 'Content';
}

sub controller
{
    my $self = shift;
    my %opts = @_;

    my $id        = $self->{id};
    my $idHeader  = $self->default_header_id;
    my $idContent = $self->default_content_id;
    my $theme     = $opts{theme}  // 'white';
    my $url       = $opts{url}    // '';
    my $width     = $opts{width}  // '100%';
    my $height    = $opts{height} // '100%';
    
    die "URL is mandatory" unless $url;
    
    return <<"EndOfControllerData";
\$(document).ready(function () {
    \$("#$id").jqxExpander({ width: '$width', height: '$height', theme: '$theme' });
    
    setTimeout(function () {
        \$.ajax({
            url: '$url',
            type: 'GET',
            dataType: 'json',
            success: function (data) {
                \$("#$id").jqxExpander('setHeaderContent', data.header);
                \$("#$id").jqxExpander('setContent', data.content);
            }
        });
    }, 500);
});
EndOfControllerData
}

sub view
{
    my $self = shift;
    my $id   = $self->{id};
    my $idHeader  = $self->default_header_id;
    my $idContent = $self->default_content_id;
    
    return <<"EndOfViewData"
    <div id='$id'>
    <div id='$idHeader'>
        Loading Header...</div>
    <div id='$idContent'>
        Loading Content...</div>
    </div>
EndOfViewData
}

1;