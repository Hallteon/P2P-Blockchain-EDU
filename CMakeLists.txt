cmake_minimum_required(VERSION 3.16)

project(P2PBlockchainQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Network Concurrent)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Concurrent)
    set(QT_VERSION_MAJOR 5)
endif()

add_executable(P2PBlockchainQt
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/transaction.h
    src/transaction.cpp
    src/block.h
    src/block.cpp
    src/blockchain.h
    src/blockchain.cpp
    src/p2pnode.h
    src/p2pnode.cpp
)

target_link_libraries(P2PBlockchainQt
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
)
