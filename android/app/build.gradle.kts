import org.gradle.internal.declarativedsl.parsing.main

plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.example.engine"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.example.engine"
        minSdk = 21
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"
    }

    sourceSets {
        getByName("main") {
            assets.srcDirs(
                "../../assets"
            )
        }
    }

    buildTypes {
        debug {
            /*packaging {
                jniLibs {
                    useLegacyPackaging = true
                }
            }*/
            externalNativeBuild {
                cmake {
                    /*arguments += "-DANDROID_STL=c++_shared"
                    arguments += "-DENGINE_MEMORY_SANITIZER=TRUE"*/
                    arguments += "-DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON"
                }
            }
        }
        release {
            isMinifyEnabled = false
            isDebuggable = false
            isJniDebuggable = false
            ndk {
                isDebuggable = false
                abiFilters += listOf("armeabi-v7a", "arm64-v8a")
            }
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            signingConfig = signingConfigs.getByName("debug")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    buildFeatures {
        prefab = true
    }
    externalNativeBuild {
        cmake {
            path = file("../../CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

dependencies {
    implementation(libs.appcompat)
    implementation(libs.games.activity)
}
