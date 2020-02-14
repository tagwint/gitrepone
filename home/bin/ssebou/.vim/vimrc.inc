let s:current_file=expand("<sfile>")

let s:colorun_file = s:current_file . ".colorun"
if filereadable(s:colorun_file)
   exe 'source' s:colorun_file
endif

" Force this file to be of vim filetype
" vim: set ft=vim:
let mapleader = "\<Space>"
let s:cur_file=expand("<sfile>")
"echo 'Current file is' . s:cur_file

"THIS FILE SUPPOSED TO BE COMMON PART OF SETTINGS FOR ANYWHERE

" VX Reset fro double leader stroke to single as advised in doc
map <Leader> <Plug>(easymotion-prefix)
"nmap  S <Plug>(easymotion-sn)
nmap  W <Plug>(easymotion-w)
nmap  B <Plug>(easymotion-bd-w)

set nowrap
set number
scriptencoding utf-8
set encoding=utf-8
set laststatus=2
set statusline+=%F
set timeout timeoutlen=1000 ttimeoutlen=200
set <F23>=kj
set <F24>=jh
set listchars=eol:¬,tab:>-,extends:>,precedes:<,trail:·

inoremap <F2> <C-c>:w<CR>i<Right>
nnoremap <F2> :w<CR>
nnoremap <F24> :qa<CR>
nnoremap Q :q<CR>

inoremap <F23> <C-c><Right>
cnoremap <F23> <C-c>
vnoremap <F23> <C-c>

nmap <silent> ,/ :nohlsearch<CR>
nnoremap <F5> "=strftime("%c")<CR>P
inoremap <F5> <C-R>=strftime("%c")<CR>
cmap w!! w !sudo tee % >/dev/null

execute 'colorscheme torte'
let &rtp=$SSHVIMRP.','.&rtp
"execute 'colorscheme '.$VIMCOLOS

hi link EasyMotionTarget ErrorMsg
"hi link EasyMotionShade  Comment
"
"hi link EasyMotionTarget2First MatchParen
"hi link EasyMotionTarget2Second MatchParen
"
"hi link EasyMotionMoveHL Search

let g:slime_target = "tmux"
let g:slime_paste_file = "$HOME/.slime_paste"
"let g:slime_target = "tmate"
"let g:slime_target = "screen"

" XML folding
let g:xml_syntax_folding=1
au FileType xml setlocal foldmethod=syntax
" Do not fold by default (on open) 
set nofoldenable
" forget what it is qfor :(
nnoremap <F8> :setlocal foldexpr=(getline(v:lnum)=~@/)?0:(getline(v:lnum-1)=~@/)\\|\\|(getline(v:lnum+1)=~@/)?1:2 foldmethod=expr foldlevel=0 foldcolumn=2<CR>
set title
set wildmenu
set wildmode=longest:full,full

set viminfo='44930,\"5000,s500,r/tmp/XScratch,r/tmp/fuset/,rXScratc

"            ^^^^^------------------------------------------- number of files kept/
"                    ^^^^ ----------------------------------- max lines for register, greater will not
"                          ^^^------------------------------- size (KB) of register, greater will not saved                   
"                               ^--------------^^------------ ignore files
"
set history=10000 " commands history
let g:ansible_extra_keywords_highlight = 1

" this fixes issue described in https://github.com/vim/vim/issues/2329, it may
" be not required anymore as it might already fixed in newer version of vim -
" NEED TO CHECK THAT
autocmd BufRead scp://* :set bt=acwrite


" this makes vimx user sys clipboard
set clipboard^=unnamed,unnamedplus

" When editing over scp, undo goes down to empty buffer.
" to prevent this, suggest a command to be executed manuall on enter.
" Could not make it work with autocmd :(
" autocmd BufWinEnter scp://* :echo expand('%:p') ' is a remote file Consider :set undoreload=0 | edit command to not loose file content with undo. Use redo <CTRL+R> to rescue.'
" Drop current word down to command
" vmap ,g :s/<C-R>//
" map ,g :s/<C-R>//

" Quick One char left in INSERT mode using HH 
inoremap HH <Esc>i

" To determine the ASCII code to use, :echo char2nr("-")). 
" - is 45
" s is 115  
" S is 83
autocmd FileType sh let b:surround_115 = "${\r}"
autocmd FileType ansible,ansible_template let b:surround_115 = "{{ \r }}"
autocmd FileType ansible,ansible_template let b:surround_83 = "\"{{ \r }}\""

" Sun 26 May 2019 12:42:52 PM CEST
" https://items.sjbach.com/319/configuring-vim-right#1

nnoremap ' `
nnoremap ` '

set scrolloff=3


function! Redir(cmd)
	for win in range(1, winnr('$'))
		if getwinvar(win, 'scratch')
			execute win . 'windo close'
		endif
	endfor
	if a:cmd =~ '^!'
		let output = system(matchstr(a:cmd, '^!\zs.*'))
	else
		redir => output
		execute a:cmd
		redir END
	endif
	vnew
	let w:scratch = 1
	setlocal buftype=nofile bufhidden=wipe nobuflisted noswapfile
	call setline(1, split(output, "\n"))
endfunction

command! -nargs=1 -complete=command Redir silent call Redir(<q-args>)

" Usage:
" 	:Redir hi ............. show the full output of command ':hi' in a scratch window
" 	:Redir !ls -al ........ show the full output of command ':!ls -al' in a scratch window
"
"===================
function! Mosh_Tab_Or_Complete()
    if col('.')>1 && strpart( getline('.'), col('.')-2, 3 ) =~ '^\w'
        return "\<C-N>"
    else
        return "\<Tab>"
endfunction


let g:ycm_python_binary_path = 'python3'
let g:ycm_autoclose_preview_window_after_completion = 1
let g:ycm_min_num_of_chars_for_completion = 1




" inoremap <Tab> <C-R>=Mosh_Tab_Or_Complete()<CR>



command! -nargs=1 Silent
\   execute 'silent !' . <q-args>
\ | execute 'redraw!'

vnoremap \\\ :terminal bash -c "echo before ; haste ; echo After"<CR>
" command Go !set $1 `echo "%" | sed 's/\.c//g'` ;gcc -o $1 "%" ; chmod o+x $1; $1

function! SetDBConn()
   let g:dbconn = input("usr/pwd@host/sid: ")
   let b:dbconn = g:dbconn
endfunction

function! OmniRun() range
 let l:omniarg = 'haste'
 let l:fl = a:firstline
 let l:cl = getline(l:fl)
 let l:next = l:cl + 1
 if ( l:cl =~ '^\s*$' )
   " bypass first empty line
   let l:cl = getline( l:next )
 endif 
 if ( l:cl =~? "describe" || 
    \ l:cl =~? "show parameter" || 
    \ l:cl =~? "alter" ||
    \ l:cl =~? "set linesize" ||
    \ l:cl =~? "\\" ||
    \ l:cl =~? "select" )
  let l:omniarg = 'sqlp'
  if !exists("g:dbconn")
   let g:dbconn = input("usr/pwd@host/sid: ")
  endif 
  if l:cl =~ "select"
   let l:omniarg = 'ssql'
  endif
  if g:dbconn =~ "pg:"
   let l:omniarg = 'rpsql'
  endif
  let l:omniarg = l:omniarg . " " . g:dbconn
 endif

 let l:tmpfile = tempname()

 execute "silent normal :" . a:firstline . "," . a:lastline . "write! " . l:tmpfile . "\<CR>"  
 execute "new"  
 let l:cmd2exec = "silent normal :r !cat " . l:tmpfile . " | omnirun  " . l:omniarg . " \<cr> "
 execute l:cmd2exec
 setlocal noswapfile 
 setlocal buftype=nofile 
 setlocal bufhidden=hide 
 execute "normal ggdd"
 if ( l:omniarg == 'haste' )
   let l:buname= "XScratch:"
   let l:cmd2exec = "silent normal :file " . l:buname .  " \<cr> "
   execute l:cmd2exec
 else 
   let b:dbconn = g:dbconn
   let b:dbconnused = g:dbconn
   let b:savedviminfo=&viminfo
   let l:tstamp = strftime("%y%m%d_%H:%M:%S")
   let l:buname= "XScratch:" . substitute( b:dbconn, "/.*@", "/xxx@", "") . "_" . l:tstamp 
   let l:buname2=  substitute( l:buname, "\:pg\:", "_pg_/xxx@", "") 
   let l:cmd2exec = "silent normal :file " . l:buname2 .  " \<cr> "
   execute l:cmd2exec
 endif
 echo "deleting " . l:tmpfile

 call delete(l:tmpfile)

endfunction


function! SqpRun()
 if !exists("g:dbconn")
  let g:dbconn = input("usr/pwd@host/sid: ")
 endif 
 let l:cmd2exec = "silent normal :terminal omnirun interactive  " . g:dbconn . " \<cr> "
 execute l:cmd2exec
 let b:dbconn = g:dbconn
endfunction

" vnoremap aaa :terminal bash -c "echo before ; haste ; echo After"<CR>
" command -range AAA call Aaa()
command -range OMNI <line1>,<line2>call OmniRun()
command SetDBConn call SetDBConn()
vnoremap <Enter><Enter> :OMNI<CR>
nnoremap <Space><Space> <C-W><C-W>
command SQP  call SqpRun()
nnoremap <Space>c :SetDBConn<CR>
nnoremap <Space><Enter> :SQP<CR>
nnoremap <Space>k <C-W>k
nnoremap <Space>j <C-W>j
nnoremap <Space>f :BLines <CR>

function! DBConnInfo()
  if exists("b:dbconnused")
    return '⏿ '. b:dbconnused
  else
    if exists("g:dbconn")
      return 'G:' . g:dbconn
    endif    
  endif
endfunction

set statusline+=%=%{DBConnInfo()}
set statusline+=\|col:\ %c
" to make movemnts jump back to previous line
set whichwrap+=<,>,h,l,[,]

set splitbelow

function! DiffRegister(reg1, reg2)
 " execute "new"  
 " let l:cmd2exec = "silent normal :r !cat " . l:tmpfile . " | omnirun  " . l:omniarg . " \<cr> "

 " let l:cmd2exec = "silent normal :new /tmp/LSide | silent normal ggdG | $put " . a:reg1 . " | x  \<cr> "
 " execute l:cmd2exec
 " let l:cmd2exec = "silent normal :new /tmp/RSide | silent normal ggdG | $put " . a:reg2 . " | x  \<cr> "
 " execute l:cmd2exec
 let l:cmd2exec = "silent normal :new | $put " . a:reg1 . " | w! /tmp/LSide  \<cr> "
 execute l:cmd2exec
 let l:cmd2exec = "silent normal :new | $put " . a:reg2 . " | w! /tmp/RSide  \<cr> "
 execute l:cmd2exec
 " execute "silent !meld /tmp/LSide /tmp/RSide" 
 execute " :terminal bash -c \"meld /tmp/LSide /tmp/RSide\" " 
 " echo 'Registers names: ' . a:reg1 . " " . a:reg2
 " execute " :terminal bash -c \"hdiff /tmp/LSide /tmp/RSide\" " 
endfunction


" makes striped highlight makes look nicer for plain files
function! SynEvenLines()
    syn clear
    highlight clear EvenLines
    for l:i in range(2,line('$')+1,2)
        exe "syn match EvenLines '\\%" . l:i . "l.*'"
    endfor
    " highlight EvenLines guibg=darkgrey ctermbg=green
    hi EvenLines ctermbg=244 guibg=#585858 
endfunction

if &diff
    map gs :call IwhiteToggle()<CR>
    function! IwhiteToggle()
      if &diffopt =~ 'iwhite'
        set diffopt-=iwhite
      else
        set diffopt+=iwhite
      endif
    endfunction
endif

" tracing of all vim-visited files by passing the name to 
" chkvimfile scipt available on path
" there it can be processed depending on file path
" that path may be specially predefined by the way vim was called (e.g. CtrlXCtrlE creates a file in /tmp 
" mutt has its own path etc. )

autocmd BufWinLeave * silent execute "!chkvimfile %:p 2>&1" | redraw!

