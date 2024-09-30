# SHEGANG Documentation: Project Architecure

```
├── build_scripts
│   ├── build_make.sh
│   ├── create_package.sh
│   ├── uninstall.sh
│   └── update.sh
├── CHANGELOG.md
├── config
├── ctypes_lib
│   ├── bin
│   ├── README.md
│   └── shegang.py
├── docs
│   ├── en
│   │   ├── article.md
│   │   ├── changes.md
│   │   ├── index.md
│   │   └── project_architecture.md
│   ├── preview.gif
│   ├── ru
│   │   ├── article.md
│   │   ├── article-part2.md
│   │   ├── index.md
│   │   └── project_architecture.md
│   └── versions
│       ├── v0.10.29b.diff
│       ├── v0.6.22a.diff
│       ├── v0.6.23a.diff
│       ├── v0.7.23a.diff
│       ├── v0.8.23a.diff.diff
│       └── v0.9.25a.diff
├── include
│   ├── autocomplete.h
│   ├── builtin.h
│   ├── colorschemes.h
│   ├── colors.h
│   ├── config.h
│   ├── environment.h
│   ├── executor.h
│   ├── fuzzy.h
│   ├── isocline.h
│   ├── plugins_manager.h
│   ├── tasks_processing.h
│   └── userinput.h
├── libisocline.a
├── LICENSE
├── log_git.py
├── Makefile
├── pipegang_plugin.so
├── plugins
│   └── pipegang_plugin.c
├── README.md
├── shegang_en.man
└── src
    ├── config
    │   └── config.c
    ├── core
    │   └── userinput.c
    ├── execution
    │   ├── builtin
    │   │   ├── base.c
    │   │   ├── gapf.c
    │   │   ├── lsblk.c
    │   │   ├── ls.c
    │   │   ├── nightswatch.c
    │   │   └── shegang_config.c
    │   ├── executor.c
    │   └── tasks_processing.c
    ├── features
    │   ├── autocomplete.c
    │   ├── colors.c
    │   ├── colorschemes.c
    │   ├── environment.c
    │   └── fuzzysearch.c
    ├── plugins
    │   └── plugins_manager.c
    └── shegang.c
```

# Content

0. [Index page](./index.md)
1. Project Architecture (YOU ARE HERE)
