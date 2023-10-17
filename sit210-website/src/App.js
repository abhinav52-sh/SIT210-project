import './App.css';
import { Routes, Route, Navigate } from 'react-router-dom';
import MainPage from './Components/MainPage';
import Login from './Components/Login';
import { useState } from 'react';


function App() {
  const [loggedIn, setLoggedIn] = useState(false);

  const loginUser = () => {
    setLoggedIn(true);
  };

  return (
    <div className="App">
      <Routes>
        <Route path='/' element={loggedIn ? <MainPage /> : <Navigate to="/login" />}/>
        <Route path='/login' element={<Login isLoggedIn={loginUser} />}/>
      </Routes>
    </div>
  );
}

export default App;
