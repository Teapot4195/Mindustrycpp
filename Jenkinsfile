pipeline {
    agent none
    stages {
        stage('Build') {
            parallel {
                stage('Linux Build amd64') {
                    agent {
                        label 'linux'
                    }

                    steps {
                        // Get the Repo
                        git branch: 'master', credentialsId: 'git-ssh', url: 'git@github.com:Teapot4195/Mindustrycpp.git'

                        // Make build directories
                        sh "mkdir -p build-release build-debug build-relwithdeb"
                        sh "mkdir -p build-archive-rel build-archive-deb build-archive-relwithdeb"
                        sh "mkdir -p artifacts"

                        // Clear the contents of the build archives and artifact directories
                        dir ("build-archive-rel") {
                            sh "rm -rf *"
                        }
                        dir ("build-archive-deb") {
                            sh "rm -rf *"
                        }
                        dir ("build-archive-relwithdeb") {
                            sh "rm -rf *"
                        }
                        dir ("artifacts") {
                            sh "rm -rf *"
                        }

                        // Build release version
                        dir ("build-release") {
                            sh '''#!/bin/bash
                            cmake -DCMAKE_BUILD_TYPE=Release ..
                            make -j $(nproc)
                            #cp -r html ../build-archive-rel
                            Files=(
                                Mindustry
                            )
                            for file in $Files; do cp $file ../build-archive-rel/$file; done;'''
                        }

                        // Build debug version
                        dir ("build-debug") {
                            sh '''#!/bin/bash
                            cmake -DCMAKE_BUILD_TYPE=Debug ..
                            make -j $(nproc)
                            #cp -r html ../build-archive-deb
                            Files=(
                                Mindustry
                            )
                            for file in $Files; do cp $file ../build-archive-deb/$file; done;'''
                        }

                        // Build relwithdeb version
                        dir ("build-debug") {
                            sh '''#!/bin/bash
                            cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
                            make -j $(nproc)
                            #cp -r html ../build-archive-relwithdeb
                            Files=(
                                Mindustry
                            )
                            for file in $Files; do cp $file ../build-archive-relwithdeb/$file; done;'''
                        }

                        // Build the artifacts
                        dir ("artifacts") {
                            sh "tar -caf mindustry-release-linux-amd64.tar.gz ../build-archive-rel"
                            sh "tar -caf mindustry-debug-linux-amd64.tar.gz ../build-archive-deb"
                            sh "tar -caf mindustry-relwithdeb-linux-amd64.tar.gz ../build-archive-relwithdeb"
                        }
                    }

                    post {
                        success {
                            archiveArtifacts artifacts: 'artifacts/*', followSymlinks: false
                        }
                    }
                }
            }
        }
    }
}