import '../App.css';
import CenteredBar from './CenteredBar';
import Header from './Header';
import FeaturedArticle from './FeaturedAricle';
import Footer from './Foot'

function MainPage() {
const imageUrls = [
    'https://th.bing.com/th/id/OIP.3erGC70DXiB_CPbGTfVubAHaEA?pid=ImgDet&rs=1',
    'https://th.bing.com/th/id/OIP.QGQYM6k0eyz1ONUgt6ftggHaE8?pid=ImgDet&w=1200&h=800&rs=1',
    'https://th.bing.com/th/id/OIP.1fg5tH7Ih7tFTTvL7URB2AHaEK?pid=ImgDet&w=1024&h=576&rs=1'
  ];
  const titles = ['Arjun', 'Abhinav', 'Aditya Dua'];
  const Methane = ['Methane  -', 'Methane  -','Methane  -']
  const CarbonMonoxide = ['CarbonMonoxide  -','CarbonMonoxide  -','CarbonMonoxide  -']
  const HeartRate = ['HeartRate -', 'HeartRate-', 'HeartRate -']


  return (
    <div className="App">

  <CenteredBar/>
  <Header/>
  <h1> WORKERS DATA </h1>
  <FeaturedArticle
        imageUrls={imageUrls}
        titles={titles}
        Methane={Methane}
        CarbonMonoxide={CarbonMonoxide}
        HeartRate={HeartRate}

      />
      <Footer/>
    </div>
  );
  }

  export default MainPage;
