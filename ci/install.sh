#!/usr/bin/env sh

# Juliana Pacheco
# University of Florida

set -ev

echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | \
    sudo tee -a /etc/apt/sources.list.d/bazel.list # tee required for sudo
curl https://bazel.build/bazel-release.pub.gpg | sudo apt-key add -
sudo apt-get update && sudo apt-get install -y bazel gcc g++ clang
