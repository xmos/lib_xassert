@Library('xmos_jenkins_shared_library@v0.15.1') _

getApproval()

pipeline {
  agent none
  //Tools for AI verif stage. Tools for standard stage in view file
  parameters {
     string(
       name: 'TOOLS_VERSION',
       defaultValue: '15.0.2',
       description: 'The tools version to build with (check /projects/tools/ReleasesTools/)'
     )
   }
  stages {
    stage('Standard build and XS2 tests') {
      agent {
        label 'x86_64&&brew'
      }
      environment {
        REPO = 'lib_xassert'
        VIEW = getViewName(REPO)
      }
      options {
        skipDefaultCheckout()
      }
      stages{
        stage('Get view') {
          steps {
            xcorePrepareSandbox("${VIEW}", "${REPO}")
          }
        }
        stage('Library checks') {
          steps {
            xcoreLibraryChecks("${REPO}")
          }
        }
        stage('Tests') {
          steps {
            runXmostest("${REPO}", 'tests')
          }
        }
        stage('xCORE builds') {
          steps {
            dir("${REPO}") {
              dir("${REPO}") {
                runXdoc('doc')
              }
              //Build these individually (or we can extend xcoreAllAppsBuild to support an argument
              dir('tests/assert_test'){
                runXmake(".", "", "XCOREAI=1")
                stash name: 'assert_test', includes: 'bin/xcoreai/*.xe'
              }
              dir('tests/assert_test_unit'){
                runXmake(".", "", "XCOREAI=1")
                stash name: 'assert_test_unit', includes: 'bin/xcoreai/*.xe'
              }
              dir('tests/fail_test'){
                runXmake(".", "", "XCOREAI=1")
                stash name: 'fail_test', includes: 'bin/xcoreai/*.xe'
              }
              dir('tests/unreachable_test'){
                runXmake(".", "", "XCOREAI=1")
                stash name: 'unreachable_test', includes: 'bin/xcoreai/*.xe'
              }
            }
          }
        }
      }// stages
      post {
        cleanup {
          xcoreCleanSandbox()
        }
      }
    }// Stage standard build

    stage('xcore.ai Verification'){
      agent {
        label 'xcore.ai-explorer'
      }
      environment {
        // '/XMOS/tools' from get_tools.py and rest from tools installers
        TOOLS_PATH = "/XMOS/tools/${params.TOOLS_VERSION}/XMOS/xTIMEcomposer/${params.TOOLS_VERSION}"
      }
      stages{
        stage('Install Dependencies') {
          steps {
            sh '/XMOS/get_tools.py ' + params.TOOLS_VERSION
            installDependencies()
          }
        }
        stage('xrun'){
          steps{
            toolsEnv(TOOLS_PATH) {  // load xmos tools
              //Run this and diff against expected output. Note we have the lib files here available
              unstash 'assert_test'
              sh 'xrun --io --id 0 bin/xcoreai/assert_test.xe &> assert_test.txt'
              sh 'cat assert_test.txt && diff assert_test.txt tests/assert_test.expect'
              
              unstash 'assert_test_unit'
              sh 'xrun --io --id 0 bin/xcoreai/assert_test_unit.xe &> assert_test_unit.txt'
              sh 'cat assert_test_unit.txt && diff assert_test_unit.txt tests/assert_test_unit.expect'
              
              unstash 'fail_test'
              sh 'xrun --io --id 0 bin/xcoreai/fail_test.xe &> fail_test.txt'
              sh 'cat fail_test.txt && diff fail_test.txt tests/fail_test.expect'
              
              unstash 'unreachable_test'
              sh 'xrun --io --id 0 bin/xcoreai/unreachable_test.xe &> unreachable_test.txt'
              sh 'cat unreachable_test.txt && diff unreachable_test.txt tests/unreachable_test.expect'

            }
          }
        }
      }//stages
      post {
        cleanup {
          cleanWs()
        }
      }
    }// xcore.ai

    stage('Update view files') {
      agent {
        label 'x86_64&&brew'
      }
      when {
        expression { return currentBuild.result == "SUCCESS" }
      }
      steps {
        updateViewfiles()
      }
    }
  }
}
