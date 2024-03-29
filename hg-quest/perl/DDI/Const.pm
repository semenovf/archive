package JQ::DDI::Const;
use strict;
use base 'Exporter';
our @EXPORT = qw(
    DDI_TYPE_INT
    DDI_TYPE_FLOAT
    DDI_TYPE_TIME
    DDI_TYPE_BOOL
    DDI_TYPE_DATE
    DDI_TYPE_DATETIME
    DDI_TYPE_TIMESTAMP
    DDI_TYPE_TEXT
    DDI_TYPE_REF
    DDI_TYPE_INDEX_UNIQUE

    DDI_MIN_INT
    DDI_MAX_INT
    DDI_MIN_FLOAT
    DDI_MAX_FLOAT
    DDI_TEXT_MAXLEN

    DDI_INDEX_TYPE_BTREE
    DDI_INDEX_TYPE_HASH
    DDI_INDEX_TYPE_RTREE
    DDI_INDEX_KIND_UNIQUE
    DDI_INDEX_KIND_FULLTEXT
    DDI_INDEX_KIND_SPATIAL
);

sub DDI_TYPE_BOOL      {1}
sub DDI_TYPE_INT       {2}
sub DDI_TYPE_FLOAT     {3}
sub DDI_TYPE_TIME      {4}
sub DDI_TYPE_DATE      {5}
sub DDI_TYPE_DATETIME  {6}
sub DDI_TYPE_TIMESTAMP {7}
sub DDI_TYPE_TEXT      {8}
sub DDI_TYPE_REF       {9}
sub DDI_TYPE_INDEX_UNIQUE {10}

sub DDI_MIN_INT        { sub {'DDI_MIN_INT'} }
sub DDI_MAX_INT        { sub {'DDI_MAX_INT'} }
sub DDI_MIN_FLOAT      { sub {'DDI_MIN_FLOAT'} }
sub DDI_MAX_FLOAT      { sub {'DDI_MAX_FLOAT'} }

sub DDI_TEXT_MAXLEN    { sub {'DDI_TEXT_MAXLEN'} }

sub DDI_INDEX_TYPE_BTREE { 'BTREE' }
sub DDI_INDEX_TYPE_HASH  { 'HASH' }
sub DDI_INDEX_TYPE_RTREE { 'RTREE' }

sub DDI_INDEX_KIND_UNIQUE   { 'UNIQUE' }
sub DDI_INDEX_KIND_FULLTEXT { 'FULLTEXT' }
sub DDI_INDEX_KIND_SPATIAL  { 'SPATIAL' }

1;
