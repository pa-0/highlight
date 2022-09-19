
complete -c highlight -s B -l batch-recursive -r -d 'convert all matching files, searches subdirs (example: -B \'*.cpp\')'
complete -c highlight -s D -l data-dir -r -d 'set path to data directory'
complete -c highlight -l config-file -r -d 'set path to a lang or theme file'
complete -c highlight -s d -l outdir -r -d 'name of output directory'
complete -c highlight -s h -d 'print help and exit'
complete -c highlight -l help -xa 'syntax theme plugin config test lsp' -d 'print help or a topic description'
complete -c highlight -s i -l input -r -d 'name of single input file'
complete -c highlight -s o -l output -r -d 'name of single output file'
complete -c highlight -s P -l progress -d 'print progress bar in batch mode'
complete -c highlight -s q -l quiet -d 'suppress progress info in batch mode'
complete -c highlight -s S -l syntax -r -d 'specify type of source code or syntax file path'
complete -c highlight -l syntax-by-name -r -d 'specify type of source code by given name'
complete -c highlight -l syntax-supported -d 'test if the given syntax can be loaded'
complete -c highlight -s v -l verbose -d 'print debug info; repeat to show more information'
complete -c highlight -l force -d 'generate output if input syntax is unknown'
complete -c highlight -l list-scripts -xa 'langs themes plugins' -d 'list installed scripts with given type'
complete -c highlight -l list-cat -r -d 'filter the scripts by the given categories (example: --list-cat=\'source;script\')'
complete -c highlight -l max-size -r -d 'set maximum input file size (examples: 512M, 1G; default: 256M)'
complete -c highlight -l plug-in -r -d 'execute Lua plug-in script; repeat option to execute multiple plug-ins'
complete -c highlight -l plug-in-param -r -d 'set plug-in input parameter'
complete -c highlight -l print-config -d 'print path configuration'
complete -c highlight -l print-style -d 'print stylesheet only (see --style-outfile)'
complete -c highlight -l skip -r -d 'ignore listed unknown file types (Example: --skip=\'bak;c~;h~\')'
complete -c highlight -l stdout -d 'output to stdout (batch mode, --print-style)'
complete -c highlight -l validate-input -d 'test if input is text, remove Unicode BOM'
complete -c highlight -l version -d 'print version and copyright information'
complete -c highlight -s O -l out-format -xa 'html xhtml latex tex odt rtf ansi xterm256 truecolor bbcode pango svg' -d 'output file in given format'
complete -c highlight -s c -l style-outfile -r -d 'name of style file or print to stdout, if \'stdout\' is given as file argument'
complete -c highlight -s e -l style-infile -r -d 'to be included in style-outfile (deprecated) use a plug-in file instead'
complete -c highlight -s f -l fragment -d 'omit document header and footer'
complete -c highlight -s F -l reformat -xa 'allman gnu google horstmann java kr linux lisp mozilla otbs pico vtk ratliff stroustrup webkit whitesmith' -d 'reformats and indents output in given style'
complete -c highlight -s I -l include-style -d 'include style definition in output file'
complete -c highlight -s J -l line-length -r -d 'line length before wrapping (see -V, -W)'
complete -c highlight -s j -l line-number-length -r -d 'line number width incl'
complete -c highlight -l line-range -r -d 'output only lines from number <start> to <end>'
complete -c highlight -s k -l font -r -d 'set font (specific to output format)'
complete -c highlight -s K -l font-size -d 'set font size (specific to output format)'
complete -c highlight -s l -l line-numbers -d 'print line numbers in output file'
complete -c highlight -s m -l line-number-start -r -d 'start line numbering with cnt (assumes -l)'
complete -c highlight -s s -l style -r -d 'set colour style (theme) or theme file path'
complete -c highlight -s t -l replace-tabs -r -d 'replace tabs by <num> spaces'
complete -c highlight -s T -l doc-title -r -d 'document title'
complete -c highlight -s u -l encoding -r -d 'set output encoding which matches input file encoding; omit encoding info if set to NONE'
complete -c highlight -s V -l wrap-simple -d 'wrap lines after 80 (default) characters w/o indenting function parameters and statements'
complete -c highlight -s W -l wrap -d 'wrap lines after 80 (default) characters'
complete -c highlight -l wrap-no-numbers -d 'omit line numbers of wrapped lines (assumes -l)'
complete -c highlight -s z -l zeroes -d 'pad line numbers with 0\'s'
complete -c highlight -l isolate -d 'output each syntax token separately (verbose output)'
complete -c highlight -l keep-injections -d 'output plug-in injections in spite of -f'
complete -c highlight -l kw-case -xa 'upper lower capitalize' -d 'change case of case insensitive keywords to given case'
complete -c highlight -l no-trailing-nl -xa 'empty-file' -d 'omit trailing newline'
complete -c highlight -l no-version-info -d 'omit version info comment'
complete -c highlight -s a -l anchors -d 'attach anchor to line numbers'
complete -c highlight -s y -l anchor-prefix -r -d 'set anchor name prefix'
complete -c highlight -s N -l anchor-filename -d 'use input file name as anchor prefix'
complete -c highlight -s C -l print-index -d 'print index with hyperlinks to output files'
complete -c highlight -s n -l ordered-list -d 'print lines as ordered list items'
complete -c highlight -l class-name -r -d 'set CSS class name prefix; omit class name if set to NONE'
complete -c highlight -l inline-css -d 'output CSS within each tag (verbose output)'
complete -c highlight -l enclose-pre -d 'enclose fragmented output with pre tag (assumes -f)'
complete -c highlight -s b -l babel -d 'disable Babel package shorthands'
complete -c highlight -s r -l replace-quotes -d 'replace double quotes by \\dq{}'
complete -c highlight -l beamer -d 'adapt output for the Beamer package'
complete -c highlight -l pretty-symbols -d 'improve appearance of brackets and other symbols'
complete -c highlight -l page-color -d 'include page color attributes'
complete -c highlight -s x -l page-size -xa 'a3 a4 a5 b4 b5 b6 letter' -d 'set page size'
complete -c highlight -l char-styles -d 'include character stylesheets'
complete -c highlight -l height -d 'set image height (units allowed)'
complete -c highlight -l width -d 'set image width (see --height)'
complete -c highlight -l canvas -d 'set background colour padding (default: 80)'
complete -c highlight -l ls-profile -r -d 'read LSP configuration from lsp.conf'
complete -c highlight -l ls-delay -r -d 'set server initialization delay'
complete -c highlight -l ls-exec -r -d 'set server executable name'
complete -c highlight -l ls-option -r -d 'set server CLI option (can be repeated)'
complete -c highlight -l ls-hover -d 'execute hover requests (HTML output only)'
complete -c highlight -l ls-semantic -d 'retrieve semantic token types (requires LSP 3.16)'
complete -c highlight -l ls-syntax -r -d 'set syntax which is understood by the server'
complete -c highlight -l ls-syntax-error -d 'retrieve syntax error information (assumes --ls-hover or --ls-semantic)'
complete -c highlight -l ls-workspace -r -d 'set workspace directory to init'
