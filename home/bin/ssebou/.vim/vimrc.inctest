" Vim GoDiff 1.2 April 2016
" A plugin to quickly compare to sections of text with colors
" Installation:
"     Source this file
"   or
"     Copy its content into your .vimrc
"   or
"     If using pathogen, copy this file into .vim/bundle/godiff/plugin/
"   or
"     Copy it to your .vim/plugin directory to automatically source it when vim
"     starts
"   or
"     Copy it to your .vim/plugins directory (or any other directory) and source
"     it explicitly from your .vimrc
" Usgae:
"   First copy text-1 (e.g. '3yy' to copy 3 lines) 
"   Then, either
"     visually select text-2 (e.g. 'V2j' to visually select 3 lines)
"     Now press 'gd' to compare text-1 and text-2
"   Or
"     give a count for number of lines to diff followed by gd (e.g. '3gd')
"   Finally press 'gd' again to restore colors
"
"   Registers can also be used. Try '"a3yy' or '3"ayy' to copy 3 lines to register 'a'
"   Then '"a3gd' or '3"agd' on text to compare with.
"
"   For single line compare, simply press 'yy' on text-1 and 'gd' on text-2
" Note:
"   If you have some personal highlights, read the comment in the end of this
"   file
" Author:
"   Rolf Asmund

let s:active = 0

function! s:Diff_LongestMatchingSubsection(a, a0, a1, b, b0, b1)
	let ret = [a:a0, a:b0, 0]
    let runs = {}
    for i in range(a:a0, a:a1-1)
        let new_runs = {}
        for j in range(a:b0, a:b1-1)
            if a:a[i] == a:b[j]
                let k = get(runs, j-1, 0) + 1
				let new_runs[j] = k
                if k > ret[2]
					let ret = [i-k+1,j-k+1,k]
				endif
			endif
		endfor
        let runs = new_runs
	endfor
	return ret
endfunction

function! s:GoDiff(register)
" simple text-diff algorithm inspired by:
" http://pynash.org/2013/02/26/diff-in-50-lines.html
	"threshold: how small sections should be considered a match?
	let threshold = 5
	" a = content of yank-/put-register (reg-0)
	" b = content of visually selected range
	let a=getreg(a:register)
	try
		let save_reg_0 = @0
		silent normal! gvy
	finally
		let b=@0
		let @0 = save_reg_0
	endtry
	" now compare the two strings a and b

	" c is result of comparison and determines the coloring of b
	" '0' = no match
	" '1' = left hand edge of match
	" '2' = match
	" '3' = right hand edge of match
	let c = repeat('0', len(b))
	" opens is a list of section pairs to be investigated.
	" initially, both strings a and b are entirely uninvestigated.
	let opens = [[0, len(a), 0, len(b)]]
	while len(opens)
		" take one section pair out of the list, and find the longest matching
		" subsection
		let a0 = opens[-1][0]
		let a1 = opens[-1][1]
		let b0 = opens[-1][2]
		let b1 = opens[-1][3]
		let opens = opens[:-2]
		let longest = <SID>Diff_LongestMatchingSubsection(a, a0, a1, b, b0, b1)
		if longest[2] > threshold
			" mark in c, how it matched
			let beg = (longest[0] == 0 && longest[1] == 0) ? '2' : '1'
			let end = (longest[0]+longest[2] == len(a) && longest[1]+longest[2] == len(b)) ? '2' : '3'
			let start = (longest[1] == 0) ? '' : c[0:longest[1]-1]
			let c = start . beg . repeat('2', longest[2]-2) . end . c[longest[1] + longest[2]:-1]
			" append sections on both sides of the match to the list of
			" sections to be investigated
			if longest[0] - a0 > threshold && longest[1] - b0 > threshold
				call add(opens, [a0, longest[0], b0, longest[1]])
			endif
			if a1 - (longest[0] + longest[2]) > threshold && b1 - (longest[1] + longest[2]) > threshold
				call add(opens, [longest[0] + longest[2], a1, longest[1] + longest[2], b1])
			endif
		endif
	endwhile
	" fix-up edge matches 
	for i in range(0, len(c)-2)
		if c[i] == '3' && c[i+1] == '0' && b[i+1] != "\x0a"
			let c = c[:i-1] . '2' . c[i+1:]
		endif
		if c[i] == '0' && c[i+1] == '1' && b[i] != "\x0a"
			let c = c[:i] . '2' . c[i+2:]
		endif
	endfor
	" now apply the coloring
	" get selections start-line and start-column in l0 and c0
	let l0 = line("'<")
	let c0 = col("'<")
	" l0p and c0p is line and column and char ahead, so we colorize one char at
	" a time
	syntax clear
	let i = 0
	while b[i] == "\x0a"
		let c0 = 1
		let l0 = l0 + 1
		let i = i + 1
	endwhile
	while i < len(c)
		let c0p = c0 + 1
		let l0p = l0
		let j = i + 1
		while b[j] == "\x0a"
			let c0p = 1
			let l0p = l0p + 1
			let j = j + 1
		endwhile
		let col = c[i] == '0' ? 'Red' : (c[i] == '2' ? 'Green': 'Blue')
		execute ':syntax region ' . col . ' start="\%' . c0 . 'c\%' . l0 . 'l" end="\%' . c0p . 'c\%' . l0p . 'l"'
		let c0 = c0p
		let l0 = l0p
		let i = j
	endwhile
	hi Red term=reverse cterm=bold ctermfg=White ctermbg=Red guifg=White guibg=Red
	hi Blue term=reverse cterm=bold ctermfg=White ctermbg=Blue guifg=White guibg=Blue
	hi Green term=reverse cterm=bold ctermfg=White ctermbg=Green guifg=White guibg=Green
	let s:user_spell = &l:spell
	setl nospell
	let s:user_hlsearch = &l:hlsearch
	setl nohlsearch
	let s:active = 1
endfunction

function! s:GoDiffStop()
	if s:active
		" if active, then stop and return to normal
		if s:user_spell
			setl spell
		endif
		if s:user_hlsearch
			setl hlsearch
		endif
		syntax on
		let s:active = 0
		if exists("*g:GoDiffExitFunc")
			call g:GoDiffExitFunc()
		endif
	else
		" if not active, do a one-line diff
		let register = v:register
		if v:count > 1
			silent execute "normal! V" . (v:count - 1) . "j\<esc>"
		else
			silent execute "normal! V\<esc>"
		endif
		call s:GoDiff(register)
	endif
endfunction

" Set-up the mappings
vnoremap <silent> gd :<c-u>call <SID>GoDiff(v:register)<cr>
nnoremap <silent> gd :<c-u>call <SID>GoDiffStop()<cr>

" if you have some personal highlight settings, you should define a function
" g:GoDiffExitFunc like this:
"
"function! s:MyHighlights()
"	hi Search term=underline cterm=underline ctermfg=NONE ctermbg=NONE
"	hi Comment term=NONE cterm=NONE ctermfg=DarkGreen ctermbg=NONE
"endfunction
"
" let g:GoDiffExitFunc = function("<SID>MyHighlights")
"

