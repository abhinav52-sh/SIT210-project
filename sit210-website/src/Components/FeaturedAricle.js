import React, { useEffect, useState } from 'react';
import './FeaturedArticle.css';
import { database } from './config';
import { onValue, ref } from 'firebase/database';

function FeaturedArticle({imageUrls}) {
  const [userDetails, setUserDetails] = useState([]);
  const [userIds, setUserIds] = useState([]);

  useEffect(() => {
    const userRef = ref(database, 'userdata');

    onValue(userRef, (snapshot) => {
      const data = snapshot.val();

      if (data) {
        const users = Object.keys(data); // Get the user IDs as an array

        // Update the user IDs and details in state
        setUserIds(users);
        setUserDetails(
          users.map((userId) => ({
            Methane: data[userId].mq4,
            CarbonMonoxide: data[userId].mq9,
            HeartRate: data[userId].hrate,
          }))
        );
      }
    });
  }, []);

  return (
    <div className="FeaturedArticle">
      <div className="ArticleContainer">
        {userIds.map((userId,  index) => (
          <div key={userId} className="Article">
            <img src={imageUrls[index]} alt={'User ${userId}'} className="ArticleImage" />
            <div className="ArticleDetails">
              <h2 className="Name">User ID: {userId}</h2>
              <h3 className="data">Methane: {userDetails[index]?.Methane}</h3>
              <h3 className="data">CarbonMonoxide: {userDetails[index]?.CarbonMonoxide}</h3>
              <h3 className="data">HeartRate: {userDetails[index]?.HeartRate}</h3>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}

export default FeaturedArticle;
