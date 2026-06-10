use strict;
use Data::Dumper;
use Data::Compare;
use Storable qw(nfreeze);
use Log::Log4perl;

sub _init_logger {
    my $conf = q(
        log4perl.rootLogger=DEBUG, Screen
        log4perl.appender.Screen=Log::Log4perl::Appender::Screen
        log4perl.appender.Screen.layout=Log::Log4perl::Layout::PatternLayout
        log4perl.appender.Screen.layout.ConversionPattern=%d{yyyy-MM-dd'T'HH:mm:ss.SSSZ} %p [PID:%P] %m%n
    );
    Log::Log4perl->init( \$conf );
}

sub _get_logger {
    return Log::Log4perl->get_logger();
}

my %_MY_MAP = (
    health => {
        mask         => 0,
        multi        => 1,
        auto         => 1,
        delOnReplace => 0,

        # TODO: Remove; it is useless field as per Ser Zen
        sortBy    => 'TIMESTAMP',
        writeDiff => 0,
        cache     => 0,
        fields    => {
            BATTERY_DESIGNED_CAPACITY     => {},
            BATTERY_FULL_CHARGED_CAPACITY => {},
            CPU_USAGE                     => {},
            PHYSICAL_MEMORY_AVAILABLE     => {},
            PHYSICAL_MEMORY_TOTAL         => {},
            STORAGE_HEALTH_STATUS         => {},
            STORAGE_OPERATIONAL_STATUS    => {},
            TIMESTAMP                     => {},
            VIRTUAL_MEMORY_AVAILABLE      => {},
            VIRTUAL_MEMORY_TOTAL          => {},
        },
    },
    registry => {
        mask         => 16,
        multi        => 1,
        auto         => 1,
        delOnReplace => 1,
        sortBy       => 'NAME',
        writeDiff    => 1,
        cache        => 1,
        fields       => {
            NAME     => { cache => 1 },
            REGVALUE => { cache => 1 }
        }
    }
);

# driver code
_init_logger();
my $logger = _get_logger();
$logger->info( "My map: " . Dumper( \%_MY_MAP ) );
_iterate_map_previous();
_iterate_map_new( \%_MY_MAP );

sub _iterate_map_previous {
    for my $section ( keys %_MY_MAP ) {
        $logger->info("Iterating section: $section");

        for my $attribute ( keys %{ $_MY_MAP{$section} } ) {
            if ( $attribute eq 'fields' ) {
                for my $field ( keys %{ $_MY_MAP{$section}->{$attribute} } ) {
                    $logger->info( "Field attribute: $field -> "
                          . $_MY_MAP{$section}->{$attribute}{$field} );
                    for my $field_option (
                        keys %{ $_MY_MAP{$section}->{$attribute}{$field} } )
                    {
                        $logger->info( "Field option: $field_option -> "
                              . $_MY_MAP{$section}
                              ->{$attribute}{$field}{$field_option} );
                    }
                }
            }
            else {
                $logger->info( "Attribute: $attribute -> "
                      . $_MY_MAP{$section}->{$attribute} );
            }
        }
    }
}

sub _iterate_map_new {
    my $data_map = shift;
    for my $section ( keys %$data_map ) {
        $logger->info("Iterating section: $section");

        for my $attribute ( keys %{ $data_map->{$section} } ) {
            if ( $attribute eq 'fields' ) {
                for my $field ( keys %{ $data_map->{$section}{$attribute} } ) {
                    $logger->info( "Field attribute: $field -> "
                          . $data_map->{$section}{$attribute}{$field} );
                    for my $field_option (
                        keys %{ $data_map->{$section}{$attribute}{$field} } )
                    {
                        $logger->info( "Field option: $field_option -> "
                              . $data_map->{$section}{$attribute}{$field}
                              {$field_option} );
                    }
                }
            }
            else {
                $logger->info( "Attribute: $attribute -> "
                      . $data_map->{$section}{$attribute} );
            }
        }
    }
}
