echo off

echo "Pulling Aperture UI's reqired submodules."

cmd git submodule update --recursive --force

cd Code/ThirdParty/sentry-native

cmd git submodule update --recursive --force