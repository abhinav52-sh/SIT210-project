import React from 'react';
import './CenteredBar.css';

const MenuItems = [

  {
    title: "About",
    url: "/plan",
    cName: "nav-links",
    icon: "fa-solid fa-briefcase"
  },

  // {
  //   title: "Switch to Visual view",
  //   url: "/login",
  //   cName: "nav-links-mobile"
  // },
];

function CenteredBar() {
  return (
    <nav className="NavbarItems">
      <div className="navbar-logo"> MineSafe Inovators</div>
      <div className="CenterContent">
        <input type="text" placeholder="Search" className="SearchInput" />
      </div>
      <ul className="nav-menu">
        {MenuItems.map((item, index) => (
          <li key={index}>
            <a className={item.cName} href="/">
              <i className={item.icon}></i>
              {item.title}
            </a>
          </li>
        ))}

        {/* Add the search bar here */}
        <li>
          <a className="nav-search" href="/">
            <i className="fas fa-search"></i>
            <button>Switch to Visual view</button>
          </a>
        </li>
      </ul>
    </nav>
  );
}

export default CenteredBar;