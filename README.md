
# Minishell

¡Un shell simple implementado en C que replica funcionalidades básicas de bash!

## 🔧 Compilación

```bash
make
```

## 🚀 Uso

```bash
# Modo interactivo
./minishell

# Modo comando
./minishell -c "comando"
```

## ✨ Características

- **Prompt personalizado** con usuario, hostname, ruta y branch de git
- **Historial** de comandos funcional
- **Expansión** de variables de entorno (`$VAR`, `$?`, `$0`, `$$` y `$_`)
- **Redirecciones y Heredoc**: `<`, `>`, `>>`, `<<`
- **Pipes** para encadenar comandos
- **Builtins**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Señales**: Control de `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
- **Comillas**: Manejo de `'` y `"` para proteger metacaracteres

---

Creación de archivos como caracteres delimitadores del tokenizer
no se crean los archivos si falla la ejecución de las redirecciones (comporbar con bash)
leaks con el heredoc y los buitins
cerramos demás una vez los fd por cada heredoc en ejecucción
faltan variables locales
ajusta $_ para que tenga un pseudo-path
ejecutar con "env -i" (sin enviroment)
ajustar, en el clusetr, los exit status a la versión de bash de los ordenadores

ejustar el env para que compile con -i, arreglando $_ y permitiendo env env, env ls, env -i etc...

---

```bash
\n
 

>
>>
<>
>>>>>
>>>>>>>>>>>>>>>
<<<
<<<<<<<<<<<<<<
> > > >
>> >> >> >>
>>>> >> >> >>
/
//
/.
/./../../../../..
///////
-
|
| hola
| | |
>>|>
""
"hola"
'hola'
''
.
..
~
ABC=hola
4ABC=hola
hola
hola que tal
Makefile
echo
echo -n
echo Hola
echoHola
echo-nHola
echo -nHola
echo Hola -n
echo Hola Que Tal
echo Hola
echo Hola Que Tal
echo \n hola
echo " " | cat -e
echo | cat -e
""''echo hola""'''' que""'' tal""''
echo -n -n
echo -p
echo -nnnnn
echo -n -nnn -nnnn
echo -n-nnn -nnnn
echo --------n
echo $
echo $?
echo $?$
echo $? | echo $? | echo $?
echo $:$= | cat -e
echo " $ " | cat -e
echo ' $ ' | cat -e
echo my shit terminal is [$TERM]
echo my shit terminal is [$TERM4
echo my shit terminal is [$TERM4]
echo $UID
echo $HOME9
echo $9HOME
echo $HOME%
echo $UID$HOME
echo Le path de mon HOME est $HOME
echo $USER$var\$USER$USER\$USERtest$USER
echo -nnnn $hola
echo > 
echo | |
EechoE
.echo.
>echo>
<echo
>>echo>>
|echo|
|echo -n hola
echo $""
echo "$"""
echo '$'''
echo $"HOME"
echo $''HOME
echo $""HOME
echo "$HO"ME
echo '$HO'ME
echo "$HO""ME"
echo '$HO''ME'
echo "'$HO''ME'"
echo ""$HOME
echo "" $HOME
echo ''$HOME
echo '' $HOME
echo $"HO""ME"
echo $'HO''ME'
echo $'HOME'
echo "$"HOME
echo $=HOME
echo $"HOLA"
echo $'HOLA'
echo $DONTEXIST Hola
echo "hola"
echo 'hola'
echo ''hola''
echo ''h'o'la''
echo "''h'o'la''"
echo "'"h'o'la"'"
echo"'hola'"
echo "'hola'"
echo '"hola"'
echo '''ho"''''l"a'''
echo hola""""""""""""
echo hola"''''''''''"
echo hola''''''''''
echo hola'""""""""""'
e"cho hola"
e'cho hola'
echo "hola " | cat -e
echo ""hola
echo "" hola
echo "" hola
echo ""hola
echo "" hola
echo hola""bonjour
"e"'c'ho 'b'"o"nj"o"'u'r
""e"'c'ho 'b'"o"nj"o"'u'r"
echo "$DONTEXIST"Makefile
echo "$DONTEXIST""Makefile"
echo "$DONTEXIST" "Makefile"
$?
$?$?
?$HOME
$
$HOME
$HOMEdskjhfkdshfsd
"$HOMEdskjhfkdshfsd"
'$HOMEdskjhfkdshfsd'
$DONTEXIST
$LESS$VAR
env hola
env hola que tal
env env
env env env env env
env ls
env ./Makefile
export "" HOLA=bonjour
export | grep "HOME"
export ""
export =
export %
export $?
export ?=2
export 9HOLA=
export HOL@=bonjour
export HOL\~A=bonjour
export -HOLA=bonjour
export --HOLA=bonjour
export HOLA-=bonjour
export HO-LA=bonjour
export HOL.A=bonjour
export HOL\\\$A=bonjour
export HOL}A=bonjour
export HOL{A=bonjour
export HO#LA=bonjour
export HO@LA=bonjour
export HO!LA=bonjour
export +HOLA=bonjour
export HOL+A=bonjour
exportHOLA=bonjour
export HOLA =bonjour
export HOLA = bonjour
export HOLA=bon!jour
export HOL@ 
export HOL^A
export ======
export ++++++
export _______
export export
export echo
export pwd
export cd
export unset
export sudo
export hola | unset hola | echo $?
/bin/echo
/bin/echo Hola Que Tal
/bin/cd Desktop
history
[touche du haut]
pwd
pwd hola
pwd ./hola
pwd hola que tal
pwd -p
pwd --p
pwd ---p
pwd -- p
pwd pwd pwd
pwd ls
pwd ls env
cd
cd .
cd ./
cd ./././.
cd ././././
cd ..
cd ../
cd ../..
cd ../.
cd .././././.
cd srcs
cd srcs objs
cd 'srcs'
cd "srcs"
cd '/etc'
cd /e'tc'
cd /e"tc"
cd sr
cd Makefile
cd ../minishell
cd ../../../../../../..
cd .././../.././../bin/ls
cd /
cd '/'
cd ///
cd ////////
cd '////////'
cd /minishell
cd _
cd -
cd --
cd ---
cd $HOME
cd $HOME $HOME
cd $HOME/42_works
cd "$PWD/srcs"
cd '$PWD/srcs'
cd minishell Docs crashtest.c
cd ~
cd ~/
chmod 000 minishell
ls hola
./Makefile
./minishell
env|"wc" -l
env|"wc "-l
expr 1 + 1
expr $? + $?
exit
exit exit
exit hola
exit hola que tal
exit 42
exit 000042
exit 666
exit 666 666
exit -666 666
exit hola 666
exit 666 666 666 666
exit 666 hola 666
exit hola 666 666
exit 259
exit -4
exit -42
exit -0000042
exit -259
exit -666
exit +666
exit 0
exit +0
exit -0
exit +42
exit -69 -96
exit --666
exit ++++666
exit ++++++0
exit ------0
exit "666"
exit '666'
exit '-666'
exit '+666'
exit '----666'
exit '++++666'
exit '6'66
exit '2'66'32'
exit "'666'"
exit '"666"'
exit '666'"666"666
exit +'666'"666"666
exit -'666'"666"666
exit 9223372036854775807
exit 9223372036854775808
exit -9223372036854775808
exit -9223372036854775809
cat | cat | cat | ls
ls | exit
ls | exit 42
exit | ls
echo | echo
echo hola | echo que tal
pwd | echo hola
env | echo hola
echo oui | cat -e
echo oui | echo non | echo hola | grep oui
echo oui | echo non | echo hola | grep non
echo oui | echo non | echo hola | grep hola
echo hola | cat -e | cat -e | cat -e
cd .. | echo "hola"
cd / | echo "hola"
cd .. | pwd
ifconfig | grep ":"
ifconfig | grep hola
whoami | grep $USER
ls | hola
ls | ls hola
ls | ls | hola
ls | hola | ls
ls | ls | hola | rev
ls | ls | echo hola | rev
ls -la | grep "."
ls -la | grep "'.'"
ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls
echo hola | cat | cat | cat | cat | cat | grep hola
echo hola | cat
echo hola| cat
echo hola |cat
echo hola|cat
ech|o hola | cat
cat Makefile | cat -e | cat -e
cat Makefile | grep srcs | cat -e
cat Makefile | grep srcs | grep srcs | cat -e
cat Makefile | grep pr | head -n 5 | cd file_not_exist
cat Makefile | grep pr | head -n 5 | hello
export HOLA=bonjour | cat -e | cat -e
unset HOLA | cat -e
export | echo hola
sleep 3 | sleep 3
sleep 3 | exit
exit | sleep 3
echo hey > hola

```

---