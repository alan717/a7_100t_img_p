(setq inhibit-startup-message t)
(require 'package)
(setq package-enable-at-startup nil)
(add-to-list 'package-archives
             '("melpa" . "http://melpa.org/packages/"))
(setq url-proxy-services '(("no_proxy" . "baidu.com")
                           ("http" . "192.168.0.106:8118")))
(package-initialize)
(when (not package-archive-contents)
    (package-refresh-contents))
(unless (package-installed-p 'use-package)
  (package-refresh-contents)
  (package-install 'use-package))
(require 'use-package)
 '(package-selected-packages
   (quote
    ( window-numbering nyan-mode solarized-theme  projectile cmake-mode  use-package undo-tree counsel-projectile go-mode)))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )

(package-install-selected-packages)
;;(require 'req-package)



;; config company
(load-theme 'solarized-dark t)
(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(custom-safe-themes
   (quote
    ("8aebf25556399b58091e533e455dd50a6a9cba958cc4ebb0aab175863c25b9a4" default)))
 '(package-selected-packages
   (quote
    (go-mode window-numbering use-package undo-tree solarized-theme nyan-mode counsel-projectile cmake-mode))))
(global-linum-mode 1)

(setq backup-directory-alist (quote (("." . "~/.emacs-backups"))))
(global-set-key "\C-s" 'swiper)
;;(global-set-key
( window-numbering-mode t)
;;2018;12;06
