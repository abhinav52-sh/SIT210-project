import React from 'react';
import './Footer.css';

function Foot() {
  return (
    <footer className="footer">
      <div className="footer-content">
        <div className="footer-logo">
          <img src="/images/logo_white_transparent.png" alt="MineSafe Inovators" />
        </div>
        <div className="footer-contact">
          <h3>Contact Us</h3>
          <p>Email: info@minesafeinovators.com</p>
          <p>Phone: +123-456-7890</p>
          <p>Address: 123 Mine Safety Street, Coalville, CO 12345</p>
        </div>
        <div className="footer-social">
          <h3>Follow Us</h3>
          <a href="#"><i className="fab fa-facebook"></i></a>
          <a href="#"><i className="fab fa-twitter"></i></a>
          <a href="#"><i className="fab fa-linkedin"></i></a>
        </div>
      </div>
      <div className="footer-bottom">
        <p>&copy; {new Date().getFullYear()} MineSafe Inovators. All rights reserved.</p>
      </div>
    </footer>
  );
}

export default Foot;
