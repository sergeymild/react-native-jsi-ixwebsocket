rm -rf ./IXWebSocket

git clone https://github.com/machinezone/IXWebSocket.git
cd IXWebSocket
git apply ../ixwebsocket.patch
cd ..

rm -rf ../cpp/ixwebsockets
mkdir ../cpp/ixwebsockets

mv ./IXWebSocket/ixwebsocket/*.cpp ../cpp/ixwebsocket
mv ./IXWebSocket/ixwebsocket/*.h ../cpp/ixwebsocket
rm -rf ./IXWebSocket
