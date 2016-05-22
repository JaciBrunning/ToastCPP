The Toast C++ Library is split into multiple projects in order to achieve its goal. This document gives a 
brief outline to how each component of the core library interfaces with one another.

# Overview
The Toast C++ Library is a culmination of 5 Main Components, `Toast-Core`, `Toast-Launcher`, `Toast-Bootstrap` and `Toast-Module`.

The Toast C++ Library is designed with large differences to the Toast Java Library, and is designed for more advanced design and
construction of Embedded Robot Code. In Toast C++, each module is started as a new process on the system, and communicates with 
the main process, `Toast-Bootstrap`, through shared memory. This has many benefits, most of which will not be discussed in this
document to keep it short and sweet. 

## Definitions
In this document, there are many definitions for the components of the Toast C++ library and what they contain. Some definitions
for words commonly used in this document are listed below.

| Word | Definition |
| ---- | ---------- |
| Toast Hooks | The Toast Hooks are the compatability layer between the Toast Internal Core and WPILib. This provides the hooks to WPILib in the case of an embedded launch, or hooks to Simulation in the case of a simulation environment. |
| Core | The use of the word 'Core' is used to declare the Toast "Functional" library. This includes components of the bootstrap and module framework that make Toast work. |
| Internal Core | The 'Internal Core' is the portion of Toast's library that functions inside of the Bootstrap. This is separate from  WPILib and the Toast Hooks |
| External Core | The 'External Core' is the portion of Toast's library that functions inside of the Module. This provides the portion of Toast that the developer uses |
| Shared Memory Pool | The 'Shared Memory Pool' is a block of shared memory that the Toast Internal Core and all Modules have access to. This is one big memory pool used for communication between all modules and processes |
| Specific Memory Pool | The 'Specific Memory Pool', also referred to as the 'Private Memory Pool' is a block of shared memory allocated between the Toast Internal Core (parent process) and a single Module process. This is used for module-specific information |

# Toast-Core
The Toast-Core is used by the Bootstrap and Module, but not by the Internals or Launcher. The purpose of this library is to provide
the common utilities, structures and definitions for use between the Modules and Bootstrap of Toast. This library is standalone and
has no dependencies.

# Toast-Launcher
The Toast-Launcher has 2 main uses. The first is to provide the initial, launchable wrapper around the `Toast-Bootstrap` library which
will be discussed later in the document. The second is to provide the subsequent launchable wrapper for each `Toast-Module`, which
will also be discussed later in the document. Because, in most cases, Toast-Launcher is only a launchable, executable wrapper, it is
very small and not complicated.

# Toast-Bootstrap
The Toast-Bootstrap is the main internal core of Toast. The Bootstrap is responsible for the Toast <--> WPILib hardware interface, shared memory
creation and loading of the modules. The Bootstrap searches for modules and loads them by allocating their specific shared memory pool and starting
the Toast-Launcher to start the module process. From there on, the Toast-Module library is responsible for linking back to the Bootstrap to mark
the module as valid and working. If, at any point, the module becomes inoperable (i.e. the module child process stops), the Bootstrap will attempt
to restart the module. Certain modules can choose to stay dead in the event of a crash, however.

# Toast-Module
The Toast-Module library is the library that module developers are exposed to. This library, called by the Toast-Launcher when the 
module is discovered, is responsible for linking back to the Bootstrap's shared and specific shared memory pools, and for reporting
all updates to the Bootstrap. This is the linkage between the Toast parent process and the module child process, and also supplies
a linkage to Toast-Common for modules to use. The Toast-Module is, in most cases, what module developers refer to as 'Toast' itself.