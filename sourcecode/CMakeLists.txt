cmake_minimum_required(VERSION 3.20)


project(MusicPlayer VERSION 1.0)


add_executable(main
    main.cpp
    ncurses_interface.cpp
    song_logistic.cpp
)


target_link_libraries(main PRIVATE ncursesw)
