<p align="center">
<img width="96" src="res/icon/icon.png" style="image-rendering: pixelated; "alt=" Fortress logo">
</p>

<h1 align="center">Welcome to the Fortress!</h1>

<h4 align="center">This is the source for Fortress framework, a study framework being created in c++ to develop games and tools for games.</h4>

<p align="center">
<a href="LICENSE"><img src="https://img.shields.io/github/license/bigasdev/fortress_framework.svg" alt="License"></a>
</p>

> 🚨 **Alert** This is a study project, i'm using it to build my games/tools but its not intended to be a working product for use (yet!), so use it with _caution_ and don't expect things to work properly out of the box.

### How to enter the fortress?

Just click the use this template button in the github page to quickly clone the framework for your own repo. The framework works best with a combo of visual studio code + [commandbar extension](https://marketplace.visualstudio.com/items?itemName=gsppvo.vscode-commandbar) with custom commands to setup and build the project.

![command-bar](media/commandbar.png)
This is what the architecture looks like:

```
└── root
    └── .build
        ├── res
        ├── game_debug.exe
        ├── dlls
    └── .release
        ├── res
        ├── game.exe
        ├── dlls
    └── res
    └── src
```

You can look at the [Twitter gif maker](https://github.com/bigasdev/Twitter-Gif-Maker) source code for a exampe of a complete application created with the framework.
