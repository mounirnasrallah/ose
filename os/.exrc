if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <S-Tab> 
cmap <S-Insert> <MiddleMouse>
map  
map ; .
map DT :.!~/franck/bin/dt
map FM o:.!~/bin/afmA
map dt :.!date +"\%F \%X"
nmap gx <Plug>NetrwBrowseX
map nt :tabnew 
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
map <BS> 
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set autoread
set autowrite
set backspace=2
set backup
set equalprg=indent
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set fileformats=unix
set formatoptions=nltc1
set guifont=Monospace\ 10
set helpheight=999
set helplang=en
set history=2000
set hlsearch
set incsearch
set mouse=a
set path=.:/home/almos-user/osm1/tp5/src_sys/kernel:/home/almos-user/osm1/tp5/src_sys/cpu/mipsel:/home/almos-user/osm1/tp5/src_sys/arch/soclib:/home/almos-user/osm1/tp5/src_sys/libk
set printoptions=paper:letter
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim72,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set shiftwidth=4
set showcmd
set showmatch
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabpagemax=30
set tabstop=4
set termencoding=utf-8
set textwidth=100
set title
set viminfo='20,\"50,:40
set visualbell
set wildmenu
set window=99
" vim: set ft=vim :
