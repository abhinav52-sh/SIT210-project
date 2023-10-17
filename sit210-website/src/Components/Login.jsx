import React, { useContext, useState } from "react";
import { useNavigate } from 'react-router-dom';
import './Login.css'

function LoginPage({isLoggedIn}) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const navigate = useNavigate();

   // Define an array of username-password pairs
   const userCredentials = [
    { username: "Arjun", password: "1234" },
    { username: "Abhinav", password: "4567" },
    { username: "Aditya", password: "8910" },
    { username: "visitor", password: "0123" },
    
  ];

  const handleLogin = (e) => {
    e.preventDefault();

  // Check if the entered username and password match any of the predefined combinations
  const isValidCredentials = userCredentials.some(
      (user) => user.username === username && user.password === password
      );
  
  
  if (isValidCredentials) {
    isLoggedIn();
      // If they match, allow access and redirect
    navigate("/");
    alert("Login Successfull")
  } else {
    // If they don't match, display an error message
    alert("Invalid username or password");
  }
};

  return (
    <div className="login-page">
      <h1>Coal Mine Hazard Prevention and Monitoring System </h1>
      <h2>User Authentication</h2>
      <form onSubmit={handleLogin}>
        <div className="form-group">
          <label>Username</label>
          <input
            type="text"
            placeholder="Username"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            required
          />
        </div>
        <div className="form-group">
          <label>Password</label>
          <input
            type="password"
            placeholder="Password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            required
          />
        </div>
        <button type="submit">Login</button>
      </form>

    </div>
  );
}

export default LoginPage;
