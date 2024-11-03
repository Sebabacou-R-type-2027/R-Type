target_sources(game PUBLIC
        FILE_SET CXX_MODULES
        FILES projectiles.cppm
            enemies.cppm
            buttons.cppm
            stats.cppm
            inputs.cppm
            settings.cppm
            network.cppm
            map_editor.cppm
            spawn_waves.cppm
)
