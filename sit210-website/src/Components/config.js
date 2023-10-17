
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";


const firebaseConfig = {
  apiKey: "AIzaSyA9yU3PxFxakuSXUdL_e5L-WAdYMH-hj48",
  authDomain: "sit210-minesafe-innovators.firebaseapp.com",
  databaseURL: "https://sit210-minesafe-innovators-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "sit210-minesafe-innovators",
  storageBucket: "sit210-minesafe-innovators.appspot.com",
  messagingSenderId: "349958473387",
  appId: "1:349958473387:web:9d68f10f91e16e7a0119ee"
};

// Initialize Firebase for Realtime Database
const app = initializeApp(firebaseConfig);
export const database = getDatabase(app);

