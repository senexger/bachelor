(TeX-add-style-hook
 "thesis2"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "12pt" "a4paper" "ngerman")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8") ("geometry" "paper=a4paper" "left=25mm" "right=25mm" "top=25mm" "bottom=25mm") ("babel" "german") ("color" "usenames" "dvipsnames")))
   (add-to-list 'LaTeX-verbatim-environments-local "lstlisting")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "lstinline")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "lstinline")
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art12"
    "inputenc"
    "subfigure"
    "geometry"
    "babel"
    "color"
    "graphicx"
    "fancyhdr"
    "float"
    "amsmath"
    "amssymb"
    "exscale"
    "subfig"
    "booktabs"
    "ulem"
    "listings"
    "tikz"
    "pgfplots"
    "relsize"
    "tabulary")
   (LaTeX-add-color-definecolors
    "mygreen"
    "mygray"
    "mymauve"))
 :latex)

