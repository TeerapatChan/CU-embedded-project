// Import the functions you need from the SDKs you need
import { initializeApp } from 'firebase/app';
import { useObjectVal } from 'react-firebase-hooks/database';
import { getDatabase, ref, set } from 'firebase/database';
// import { getFirestore } from 'firebase/firestore/lite';
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyAQg815KBuHMIlC4zFU8MP9RrF9QUUDORo",
  authDomain: "embedded-4affd.firebaseapp.com",
  databaseURL: "https://embedded-4affd-default-rtdb.asia-southeast1.firebasedatabase.app/",
  projectId: "embedded-4affd",
  storageBucket: "embedded-4affd.appspot.com",
  messagingSenderId: "43755627940",
  appId: "1:43755627940:web:8c7930a34dab98c26c8d31",
  measurementId: "G-5LWT4CDGZ1"
};

// Initialize Firebase
const firebaseApp = initializeApp(firebaseConfig);
export const database = getDatabase(firebaseApp);
// var db = firebase.firestore();
// const db = getFirestore(app);
 
// export default db;
// export default firebase;