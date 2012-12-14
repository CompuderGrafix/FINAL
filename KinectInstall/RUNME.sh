#!/bin/bash
is64bit(){ if [ "`uname -a`" == 'x86_64' ]; then return 1; fi; return 0; }
getopenni()
{
    if is64bit; then
      wget http://www.emccann.net/.openni/openni-bin-dev-linux-x64-v1.5.4.0.tar.bz2
    else
      wget http://www.emccann.net/.openni/openni-bin-dev-linux-x86-v1.5.4.0.tar.bz2
    fi
    tar -xvf openni*.tar.bz2
    mv OpenNI*/ OpenNI
    sudo rm ~/kinect/OpenNI -Rf 2> /dev/null
    mv OpenNI ~/kinect/
    rm openni*.tar.bz2
}
getnite()
{
    if is64bit; then
      wget http://www.emccann.net/.openni/nite-bin-linux-x64-v1.5.2.21.tar.bz2
    else
      wget http://www.emccann.net/.openni/nite-bin-linux-x86-v1.5.2.21.tar.bz2
    fi
    tar -xvf nite*.tar.bz2
    mv NITE*/ NITE
    sudo rm ~/kinect/NITE -Rf 2> /dev/null
    mv NITE ~/kinect/
    rm nite*.tar.bz2
}
getsensorkinect()
{
    wget https://github.com/avin2/SensorKinect/zipball/unstable
    unzip unstable
    sudo rm ~/kinect/sensorkinect -Rf 2> /dev/null
    mv avin* sensorkinect
    mv -f sensorkinect ~/kinect/
    rm unstable
}
getsensorxtion()
{
    if is64bit; then
      wget http://www.emccann.net/.openni/sensor-bin-linux-x64-v5.1.0.41.tar.bz2
    else
      wget http://www.emccann.net/.openni/sensor-bin-linux-x86-v5.1.0.41.tar.bz2
    fi
    tar -xvf sensor-bin*.tar.bz2
    mv Sensor-Bin*/ sensorprimesense
    sudo rm ~/kinect/sensorprimesense -Rf 2> /dev/null
    mv sensorprimesense ~/kinect/
    rm sensor-bin*.tar.bz2    
}
installopenni()
{
    cd ~/kinect/OpenNI
    chmod +x install.sh
    sudo ./install.sh
}
installnite()
{
    cd ~/kinect/NITE/
    chmod a+x install.sh
    sudo ./install.sh
}
installsensorkinect()
{
    cd ~/kinect/sensorkinect/Platform/Linux/CreateRedist/
    chmod a+x RedistMaker
    sudo ./RedistMaker
    if is64bit; then    
      cd ../Redist/Sensor-Bin-Linux-x64*
    else
      cd ../Redist/Sensor-Bin-Linux-x86*    
    fi
    sudo chmod a+x install.sh
    sudo ./install.sh
}
installsensorxtion()
{
    cd ~/kinect/sensorprimesense/
    chmod a+x install.sh
    sudo ./install.sh
}
#clean up shop from canceled previous installs
rm -Rf OpenNI* openni* nite* NITE* sensor* 
mkdir -p ~/kinect
getopenni
getnite
getsensorkinect
getsensorxtion
pushd . >& /dev/null
installopenni
installsensorkinect
installnite
installsensorxtion
popd >& /dev/null
