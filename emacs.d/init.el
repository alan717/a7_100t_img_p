;;; package-sumary:

(require 'package)
;;;code
(add-to-list 'package-archives
         '("melpa" . "http://melpa.org/packages/") t)
(add-to-list 'load-path "~/.emacs.d/my/")

(require 'powerline)
;;;(require 'smex)

(package-initialize)

(when (not package-archive-contents)
    (package-refresh-contents))

(unless (package-installed-p 'use-package)
  (package-install 'use-package))

(require 'use-package)
(setq use-package-always-ensure t)
(require 'req-package)




(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(custom-safe-themes
   (quote
    ("e61752b5a3af12be08e99d076aedadd76052137560b7e684a8be2f8d2958edc3" "13d20048c12826c7ea636fbe513d6f24c0d43709a761052adbca052708798ce3" "26d49386a2036df7ccbe802a06a759031e4455f07bda559dcf221f53e8850e69" default)))
 '(package-selected-packages
   (quote
    (company-rtags flycheck-rtags moe-theme nyan-mode solarized-theme smex org-mode projectile cmake-mode irony company-irony flycheck-irony irony-eldoc yasnippet use-package undo-tree counsel-projectile company anzu req-package flycheck))))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )

(package-install-selected-packages)
;; config company
(req-package company
	     :config
	     (progn
	       (add-hook 'after-init-hook 'global-company-mode)
	       (global-set-key (kbd "M-/") 'company-complete-common-or-cycle)
	       (setq company-idle-delay)))

(req-package flycheck
  :config
  (progn
    (global-flycheck-mode))
  )




;; (req-package irony
;;   :config
;;   (progn
;;     ;; If irony server was never installed, install it.
;;     (unless (irony--find-server-executable) (call-interactively #'irony-install-server))

;;     (add-hook 'c++-mode-hook 'irony-mode)
;;     (add-hook 'c-mode-hook 'irony-mode)

;;     ;; Use compilation database first, clang_complete as fallback.
;;     ;;(setq-default irony-cdb-compilation-databases 'irony-cdb-json)

;;     (add-hook 'irony-mode-hook 'irony-cdb-autosetup-compile-options)
;;   ))

;;   ;; I use irony with company to get code completion.
;;   (req-package company-irony
;;     :require company irony
;;     :config
;;     (progn
;;       (eval-after-load 'company '(add-to-list 'company-backends 'company-irony))))

;;   ;; I use irony with flycheck to get real-time syntax checking.
;;   (req-package flycheck-irony
;;     :require flycheck irony
;;     :config
;;     (progn
;;       (eval-after-load 'flycheck '(add-hook 'flycheck-mode-hook #'flycheck-irony-setup))))

;;   ;; Eldoc shows argument list of the function you are currently writing in the echo area.
;;   (req-package irony-eldoc
;;     :require eldoc irony
;;     :config
;;     (progn
;;       (add-hook 'irony-mode-hook #'irony-eldoc)))
;;



;;;rtags
(req-package rtags
  :config
  (progn
    (unless (rtags-executable-find "rc") (error "Binary rc is not installed!"))
    (unless (rtags-executable-find "rdm") (error "Binary rdm is not installed!"))
    (setq rtags-path '/home/ququ/.emacs.d/rtags/bin)
    (add-hook 'c-mode-hook 'rtags-start-process-unless-running)
    (add-hook 'c++-mode 'rtags-start-process-unless-running)
    
    
    (define-key c-mode-base-map (kbd "M-.") 'rtags-find-symbol-at-point)
    (define-key c-mode-base-map (kbd "M-,") 'rtags-find-references-at-point)
    (define-key c-mode-base-map (kbd "M-?") 'rtags-display-summary)
;    (define-key c-mode-base-map
    
    (rtags-enable-standard-keybindings)
    (rtags-restart-process)
    
    
;;    (setq rtags-use-helm t)

    ;; Shutdown rdm when leaving emacs.
    (add-hook 'kill-emacs-hook 'rtags-quit-rdm)
    ))

;; TODO: Has no coloring! How can I get coloring?
;; (req-package helm-rtags
;;   :require helm rtags
;;   :config
;;   (progn
;;     (setq rtags-display-result-backend 'helm)
;;     ))

;; Use rtags for auto-completion.
(req-package company-rtags
  :require company rtags
  :config
  (progn
    (setq rtags-autostart-diagnostics t)
;;    (setq rtags-autostart-diagnostics
    (rtags-diagnostics)
    (setq rtags-completions-enabled t)
    (push 'company-rtags company-backends)
    ))

;; Live code checking.
(req-package flycheck-rtags
  :require flycheck rtags
  :config
  (progn
    ;; ensure that we use only rtags checking
    ;; https://github.com/Andersbakken/rtags#optional-1
    (defun setup-flycheck-rtags ()
      (flycheck-select-checker 'rtags)
      (setq-local flycheck-highlighting-mode nil) ;; RTags creates more accurate overlays.
      (setq-local flycheck-check-syntax-automatically nil)
      (rtags-set-periodic-reparse-timeout 2.0)  ;; Run flycheck 2 seconds after being idle.
      )
    (add-hook 'c-mode-hook #'setup-flycheck-rtags)
    (add-hook 'c++-mode-hook #'setup-flycheck-rtags)
    ))
;;;;;;rtags ending ##################



(req-package projectile
  :config
  (progn
    (projectile-global-mode)
    ))




(req-package-finish)

(global-linum-mode t)
;;;init.el end here

(load-theme 'manoj-dark t)


(smex-initialize)
;;(smex t)
;;(use-package com
;;(load-theme 'solarized-dark t)
(nyan-mode t)
;;(use-package moe-theme
;;  :ensure t
;;  :config
