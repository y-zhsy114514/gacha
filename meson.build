project('gacha_random', 'cpp', version: '1.0.0', license: 'MIT', default_options: ['c_std=c++17', 'cpp_std=c++17', 'warning_level=3', 'buildtype=debugoptimized',],)
fmt = dependency('fmt', version: '>= 8.1.1')
json = dependency('nlohmann_json', version: '>= 3.11.0')
executable('gachas', 'src/main.cpp', dependencies: [fmt,json,],)
