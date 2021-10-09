man diff
man patch
vi /usr/src/build.sh  # Add an empty comment in the beginning
diff /usr/src_orig/build.sh /usr/src/build.sh > build.patch
rsync -avz minix:~/build.patch ~
patch -p0 build.sh ~/build.patch
patch -R build.sh ~/build.patch