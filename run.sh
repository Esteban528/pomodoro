#!/usr/bin/env bash

mvn clean package

java -jar target/pomodorotimer-1.0-SNAPSHOT.jar
