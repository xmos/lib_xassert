// This file relates to internal XMOS infrastructure and should be ignored by external users

@Library('xmos_jenkins_shared_library@v0.34.0') _

getApproval()

pipeline {
    agent {
        label 'x86_64 && linux'
    }
    environment {
        REPO = 'lib_xassert'
    }
    options {
        buildDiscarder(xmosDiscardBuildSettings())
        skipDefaultCheckout()
        timestamps()
    }
    parameters {
        string(
        name: 'TOOLS_VERSION',
        defaultValue: '15.3.0',
        description: 'The XTC tools version'
        )
        string(
        name: 'XMOSDOC_VERSION',
        defaultValue: 'v6.1.3',
        description: 'The xmosdoc version'
        )
        string(
            name: 'INFR_APPS_VERSION',
            defaultValue: 'v2.0.1',
            description: 'The infr_apps version'
        )
    }
    stages {
        stage('Build examples') {
            steps {
                println "Stage running on ${env.NODE_NAME}"

                dir("${REPO}") {
                    checkout scm

                    dir("examples") {
                        withTools(params.TOOLS_VERSION) {
                            sh 'cmake -G "Unix Makefiles" -B build'
                            sh 'xmake -C build -j 8'
                        }
                    }
                } // dir("${REPO}")
            } // steps
        }  // stage('Build examples')

        stage('Library checks') {
            steps {
                runLibraryChecks("${WORKSPACE}/${REPO}", "${params.INFR_APPS_VERSION}")
            }
        }

        stage('Documentation') {
            steps {
                dir("${REPO}") {
                    buildDocs()
                } // dir("${REPO}")
            } // steps
        } // stage('Documentation')

        stage('Tests') {
            steps {
                dir("${REPO}") {
                    withTools(params.TOOLS_VERSION) {
                        createVenv(reqFile: "tests/requirements.txt")
                        withVenv {
                             dir("tests") {
                                sh 'cmake -G "Unix Makefiles" -B build'
                                sh 'xmake -C build -j 8'
                                sh "pytest -n auto --junitxml=pytest_result.xml"
                            }
                        }
                    }
                }
            }
        } // stage('Tests')
    } // stages

    post {
        cleanup {
            xcoreCleanSandbox()
        }
    }
}
