# Centaur Emacs

[![Build Status](https://travis-ci.org/seagle0128/.emacs.d.svg?branch=master)](https://travis-ci.org/seagle0128/.emacs.d)
[![Release Tag](https://img.shields.io/github/tag/seagle0128/.emacs.d.svg?label=release)](https://github.com/seagle0128/.emacs.d/releases/latest)
[![License](http://img.shields.io/:license-gpl3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0.html)

![Centaur Emacs](logo.png)

This is an Emacs distribution that aims to enhance the default
Emacs experience. It alters a lot of the default settings,
bundles a plethora of additional packages and adds its own core
library to the mix. The final product offers an easy to use Emacs
configuration for Emacs newcomers and lots of additional power for
Emacs power users.

It's able to run on Windows, GNU Linux and macOS. It is compatible **ONLY with
GNU Emacs 25.1 and above**. In general you're advised to always run with the
latest stable release - currently **26.1**.

## Features

- Out of box.
- Clean and Fast.
- Quick fuzzy search.
- Better Org support.
- Support multiple programming languages
  - C/C++/C#/Java
  - Ruby/Python/Perl/PHP/Shell/Powershell
  - Javascript/Typescript/JSON/YAML
  - HTML/CSS/XML
  - Golang/Swift
  - Markdown
  - ...
- Auto completion.
- Fly syntax check.
- Fly spell check.
- Git/SVN integration.
- Projectile integration.
- Workspace integration.
- Pomodor integration.
- Youdao dictionary integration.
- Support Chinese fonts and calendar.
- Support Pinyin search.

## Prerequiste

### OS

- GNU Linux
- macOS
- Windows (Cygwin)

### GNU Emacs

Please refer to [Installing Emacs](http://wikemacs.org/index.php/Installing_Emacs).

### Dotfiles

Recommend to use [Centaur Dotfiles](https://github.com/seagle0128/dotfiles).

## Quick Start

### Install

``` shell
mv ~/.emacs.d ~/.emacs.d.bak
git clone --depth 1 https://github.com/seagle0128/.emacs.d.git ~/.emacs.d
```

or download the [zip
package](https://github.com/seagle0128/.emacs.d/archive/master.zip) directly and
extract to `~/.emacs.d`.

Then start emacs. Wait for a while to install packages at the first startup.
Enjoy!

### Update

``` emacs-lisp
M-x update-centaur
```

## Customization

### Customize-group

`M-x customize-group` and select `centaur`. Set and save the configurations,
then restart Emacs.

### Manual

Copy `custom-template.el` to `custom.el` and change the configurations, then
restart Emacs.

For Example:

``` emacs-lisp
(setq centaur-logo nil)                        ; Logo file or nil (official logo)
(setq centaur-full-name "user name")           ; User full name
(setq centaur-mail-address "user@email.com")   ; Email address
(setq centaur-proxy "127.0.0.1:1080")          ; Network proxy
(setq centaur-package-archives 'emacs-china)   ; Package repo: melpa, melpa-mirror, emacs-china, netease or tuna
(setq centaur-theme 'doom)                     ; Color theme: default, doom, dark, light or daylight
(setq centaur-dashboard t)                     ; Use dashboard at startup or not: t or nil
(setq centaur-lsp nil)                         ; Enable language servers or not: t or nil
(setq centaur-company-enable-yas t)            ; Enable yasnippet for company or not: t or nil
(setq centaur-benchmark t)                     ; Enable initialization benchmark or not: t or nil
```

The default pacakge archives is `melpa`. You can change it in `custom.el`, or
switch manually via `M-x switch-package-archives` anytime.

For the personal configurations, you could put to `~/.emacs.d/custom-post.el`.

## Screenshots

### Main (default theme)

![Main](https://user-images.githubusercontent.com/140797/30391180-20bd0ba8-987e-11e7-9cb4-2aa66a6fd69d.png)

### Programming (dark theme)

![Programming](https://user-images.githubusercontent.com/140797/31727834-433c1164-b3f0-11e7-9f73-0977d9f600f1.png)

### Org mode (dark theme)

![Org](https://user-images.githubusercontent.com/140797/30391183-20c37e8e-987e-11e7-9579-c4df71549a76.png)

### Search (light theme)

![Search](https://user-images.githubusercontent.com/140797/30391184-20f47fac-987e-11e7-8be4-9f4e409d65cc.png)

### Magit (daylight theme)

![Magit](https://user-images.githubusercontent.com/140797/30391181-20bd848e-987e-11e7-9cda-3dac2865922e.png)

### Dired and replace (doom theme with [doom-modeline](https://github.com/seagle0128/doom-modeline))

![Doom](https://user-images.githubusercontent.com/140797/41302817-13cb7622-6e9e-11e8-894b-07aff95f91bc.png)

### Dashboard (doom theme with [doom-modeline](https://github.com/seagle0128/doom-modeline))

![Dashboard](https://user-images.githubusercontent.com/140797/43999598-6514db6c-9e42-11e8-8219-05d297fbbe8d.png)

## FAQ

1. Why is the modline messy?

    Powerline fonts or all-the-icons are missing on your system. Please install
    [powerline-fonts](https://github.com/powerline/fonts) for `spaceline` or
    run `M-x all-the-icons-install-fonts` for `doom-modeline`.

1. How to search Chinese via pinyin?

    In Emacs, `C-s :`. If you just want to search `:`, use `C-s \:`.

1. How to use the Centaur Dashboard?

    Set `(setq centaur-dashboard t)` in `~/.emacs.d/custom.el`. Dashboard will
    be opened at startup. After startup, you could use `F2` to reopen it anytime.
    In the dashboard, you could easily jump to Homepage(`H`), Restore
    Session(`S`), Edit Config (`E`), Update(`U`), Recent Files (`r`),
    Bookmarks(`m`) and Projects(`p`).

1. Does Centuar Emacs support Language Server Protocol (LSP)?

    LSP is supported and enabled by default in Centuar Emacs now. Please install
    language servers as below. Use `(setq centaur-lsp nil)` to disable `LSP` if
    you don't like it.
    - Golang: `go get -u github.com/sourcegraph/go-langserver`
    - Python: `pip install python-language-server`
    - Ruby:  `gem install solargraph`
    - Javascript/Typescript: `npm i -g javascript-typescript-langserver`
    - CSS: `npm i -g vscode-css-languageserver-bin`
    - HTML: `npm i -g vscode-html-languageserver-bin`
    - Bash/Shell: `npm i -g bash-language-server`. Require Python2.5+, use
      `--python` to specify.
    - C/C++/Objective-C : `brew install cquery` or dwonload binary from
      [here](https://github.com/cquery-project/cquery/releases).
    - Rust: `rustup component add rls-preview rust-analysis rust-src`
    - Java:
      ``` shell
      wget http://download.eclipse.org/jdtls/snapshots/jdt-language-server-latest.tar.gz
      tar jdt-language-server-latest.tar.gz -C ~/.emacs.d/eclipse.jdt.ls/server/
      ```
    - PHP: refer to the [installation
      guide](https://github.com/felixfbecker/php-language-server#installation).
      ``` shell
      composer require felixfbecker/language-server
      composer run-script --working-dir=vendor/felixfbecker/language-server parse-stubs
      ```

1. How to enable `plantuml` in `org-mode`?

    Put `(setq org-plantuml-jar-path "<path of plantumx.x.x.jar>")` in `custom.el`.

1. Why the Emacs environment variables and `exec-path` are different between GUI
   and terminal?

    Please refer to #33. You should instead set environment variables in startup
    files like .profile, .bash_profile or .zshenv, then `Centaur Emacs` is able
    to recoginze and import the environment variables.
