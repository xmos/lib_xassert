@Library('xmos_jenkins_shared_library@v0.34.0') _

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
      defaultValue: 'v6.1.2',
      description: 'The xmosdoc version')
  }
  stages {
    stage('Build and test') {
      agent {
        label 'documentation && linux && x86_64'
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

        stage('Library Checks') {
          steps {
            runLibraryChecks("${WORKSPACE}/${REPO}", "v2.0.1")
          } // steps
        } // stage('Library Checks')

        stage('Build Documentation') {
          steps {
            dir("${REPO}") {
              buildDocs()
            } // dir("${REPO}")
          } // steps
        } // stage('Build Documentation')

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
        } // stage('Simulator tests')

      } // stages
    }  // stage('Build and test')
  }
}
