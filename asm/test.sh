chmod u+x+r+w mul.asm
rm -r ./build
./build.sh
if [[ "$1" == "" ]]
then
runhaskell test.hs mul --bin ./build/mul
else
runhaskell test.hs $1 --bin ./build/$1
fi
