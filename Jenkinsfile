pipeline {
    agent any

    environment {
        CC = "gcc"
        CFLAGS = "-Iinclude -Wall -Werror"
        TARGET = "app"
    }

    stages {

        stage('Checkout') {
            steps {
                echo '📥 Cloning repository...'
                // Pull latest code from GitHub
                git branch: 'main', url: 'https://github.com/yourusername/my_c_project.git'
            }
        }

        stage('Build') {
            steps {
                echo '⚙️ Building project using Makefile...'
                // Clean old files and build new binary
                sh '''
                    make clean
                    make all
                '''
            }
        }

        stage('Run Program') {
            steps {
                echo '🚀 Running the built executable...'
                // Run your app (optional)
                sh '''
                    if [ -f "./app" ]; then
                        echo "Program output:"
                        ./app
                    else
                        echo "⚠️ app not found!"
                    fi
                '''
            }
        }

        stage('Archive Artifact') {
            steps {
                echo '📦 Archiving the final executable...'
                // Save app binary as artifact
                archiveArtifacts artifacts: 'app', fingerprint: true
            }
        }
    }

    post {
        success {
            echo '✅ Build completed successfully!'
        }
        failure {
            echo '❌ Build failed. Check errors above.'
        }
    }
}

