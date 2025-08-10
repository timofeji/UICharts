# UICharts
- [UICharts](#montage-graph-plugin-)
    - [About](#about)
    - [Installation](#installation)
    - [Features](#features)
    - [Showcase](#showcase)


## About
Common Slate/UMG Charts and Graphs, developed for [Eonblade](https://store.steampowered.com/app/1213840/EonBlade/). \
Contributions, feedback and criticism are more than welcome.


## Installation
### 1.)
in your Unreal Project directory run the following:
```shell
git subtree add --prefix=Plugins/UICharts git@github.com/timofeji/UICharts.git master --squash
```
or as a submodule
```shell
git submodule add git@github.com/timofeji/UICharts.git Plugins/UICharts 
```
### 2.)

in your `{ProjectName}.build.cs` add the following:
```csharp
PrivateDependencyModuleNames.AddRange(new string[]
{	
    //...
    "UICharts",
    //...
});

```

## Features
### Radar Chart:

The first UV channel is projected
### Line Graph:
### Color Picker:
  

## Links
* [💬 Discord](https://discord.gg/ymHY5eP)

## Showcase
* [EonBlade](https://store.steampowered.com/app/1213840/EonBlade/)

If you're using this plugin in one of your projects, we'd love to add you to this list 