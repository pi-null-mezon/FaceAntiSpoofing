FROM ubuntu:18.04

RUN apt-get update && apt-get install -y locales && rm -rf /var/lib/apt/lists/* \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

# Enable unicode support
ENV LANG en_US.UTF-8

# Update OS and install build tools
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y build-essential cmake git pkg-config wget 

# Build opencv
RUN apt-get install -y libtbb2 libtbb-dev libjpeg-dev libpng-dev libopenblas-dev && \
    git clone https://github.com/opencv/opencv.git opencv && cd opencv && mkdir build && cd build && \
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
          -D CMAKE_INSTALL_PREFIX=/usr/local \
          -D INSTALL_C_EXAMPLES=OFF \
          -D INSTALL_PYTHON_EXAMPLES=OFF \
          -D BUILD_EXAMPLES=OFF \
          -D BUILD_TESTS=OFF \
          -D BUILD_PERF_TESTS=OFF \
          -D BUILD_opencv_apps=OFF \
          -D BUILD_opencv_photo=OFF \
          -D BUILD_opencv_video=OFF \
		  -D BUILD_opencv_videoio=OFF \
          -D BUILD_opencv_objdetect=OFF \
          -D BUILD_opencv_flann=OFF \
          -D BUILD_opencv_highgui=OFF \
          -D BUILD_opencv_dnn=OFF \
          -D BUILD_opencv_python3=OFF \
          -D BUILD_opencv_ml=OFF \
          -D BUILD_opencv_gapi=OFF \
          -D BUILD_opencv_features2d=OFF \
          -D BUILD_opencv_ts=OFF \
          -D BUILD_opencv_java_bindings_generator=OFF \
          -D BUILD_opencv_python_bindings_generator=OFF \
          .. && \
    make && \
    make install && \
    ldconfig && \
    cd ../../ && rm -rf opencv

# Build Dlib
RUN git clone https://github.com/davisking/dlib.git dlib && \
    cd dlib && mkdir build && cd build && \
    cmake -D DLIB_NO_GUI_SUPPORT=ON \
    .. && \
    cmake --build . --config Release && \
    make install && \
    ldconfig && \
    cd ../../ && rm -rf dlib

# Install Qt5 	
RUN apt-get install -y qt5-default

# Install Python3 with webserver packages
RUN apt-get install -y python3 python3-pip && \
    pip3 install -U Flask && \
	pip3 install waitress	

# Download OpenIRT project
RUN git clone https://github.com/pi-null-mezon/OpenIRT.git

# Build oictcli 
RUN cd OpenIRT/Apps/oictcli && \
    mkdir build && cd build && \
    qmake ../oictcli.pro && \
    make && \
    make install
	
# Build spoofingsrv 
RUN cd OpenIRT/Apps/FaceAntiSpoofing/antispoofingsrv && \
    mkdir build && cd build && \
    qmake ../antispoofingsrv.pro && \
    make && \
    make install
	
# Remove unused OpenIRT parts
RUN mv OpenIRT/Apps/FaceAntiSpoofing/httpsrv/httpsrv.py . && \
	rm -rf OpenIRT
	
# Download resources 
RUN wget https://github.com/davisking/dlib-models/raw/master/shape_predictor_5_face_landmarks.dat.bz2 && \   
    bzip2 -d shape_predictor_5_face_landmarks.dat.bz2 && \
    mv shape_predictor_5_face_landmarks.dat /usr/local/bin &&\
	wget https://github.com/pi-null-mezon/FaceAntiSpoofing/raw/master/ReplayAttack/Models/replay_attack_net_v6.dat.bz2 && \
	bzip2 -d replay_attack_net_v6.dat.bz2 && \
    mv replay_attack_net_v6.dat /usr/local/bin	&&
	wget https://github.com/pi-null-mezon/FaceAntiSpoofing/raw/master/PrintAttack/Models/print_attack_net_v7.dat.bz2 && \
	bzip2 -d replay_attack_net_v7.dat.bz2 && \
    mv replay_attack_net_v7.dat /usr/local/bin	
	
# Prepare web server
RUN mkdir -p /home/Testdata && \
	printf '#!/bin/bash\nantispoofingsrv -p8080 &\npython3 httpsrv.py' > serve && \
	chmod +x serve

# This port is listening by oirtweb server by default
EXPOSE 80

CMD ./serve