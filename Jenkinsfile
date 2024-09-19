@Library('xmos_jenkins_shared_library@develop') _

getApproval()

pipeline {
  agent none
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
      defaultValue: 'v6.0.0',
      description: 'The xmosdoc version')
  }
  stages {
    stage('Build and test') {
      agent {
        label 'x86_64 && linux'
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
            }
            runLibraryChecks("${WORKSPACE}/${REPO}")
          }
        }  // Build examples

        stage('Build documentation') {
          steps {
            dir("${REPO}") {
              withXdoc("v2.0.20.2.post0") {
                withTools(params.TOOLS_VERSION) {
                  dir("${REPO}/doc") {
                    sh "xdoc xmospdf"
                    archiveArtifacts artifacts: "pdf/*.pdf"
                  }
                }
              }
            }
          }
        }  // Build documentation

        stage('Simulator tests') {
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
        }
      }
      post {
        always {
          junit "${REPO}/tests/pytest_result.xml"
        }
        cleanup {
          xcoreCleanSandbox()
        }
      }
    }  // Build and test

    stage('Hardware tests') {
      parallel {
        stage('MacOS HW tests') {
          agent {
            label 'macos && arm64 && usb_audio && xcore.ai-mcab'
          }
          steps {
            println "Stage running on ${env.NODE_NAME}"

            dir("${REPO}") {
              checkout scm
              createVenv(reqFile: "requirements.txt")
            }

            dir("${REPO}/tests/xua_hw_tests") {
              withTools(params.TOOLS_VERSION) {
                sh "cmake -G 'Unix Makefiles' -B build"
                sh "xmake -C build -j 8"

                withVenv {
                  withXTAG(["usb_audio_mc_xcai_dut"]) { xtagIds ->
                    sh "pytest -v --junitxml=pytest_hw_mac.xml --xtag-id=${xtagIds[0]}"
                  }
                }
              }
            }
          }
          post {
            always {
              junit "${REPO}/tests/xua_hw_tests/pytest_hw_mac.xml"
            }
            cleanup {
              xcoreCleanSandbox()
            }
          }
        }  // MacOS HW tests
      }
    }  // Testing
  }
}
