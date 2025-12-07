import React, { useState } from 'react';
import { criminalAPI } from '../../services/api';
import { MagnifyingGlassIcon, UserCircleIcon } from '@heroicons/react/24/outline';

const CriminalSearch = () => {
  const [searchType, setSearchType] = useState('cnic');
  const [searchQuery, setSearchQuery] = useState('');
  const [results, setResults] = useState([]);
  const [loading, setLoading] = useState(false);

  const handleSearch = async (e) => {
    e.preventDefault();
    if (!searchQuery.trim()) return;

    setLoading(true);
    try {
      let response;
      if (searchType === 'cnic') {
        response = await criminalAPI.getByCNIC(searchQuery);
        if (response.data.success && response.data.criminal) {
          setResults([response.data.criminal]);
        } else {
          setResults([]);
        }
      }
    } catch (error) {
      console.error('Search error:', error);
      setResults([]);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="bg-white rounded-xl shadow-sm p-6">
      <h2 className="text-xl font-bold text-gray-800 mb-6">Instant Criminal Search</h2>
      
      <div className="mb-6">
        <div className="flex space-x-4 mb-4">
          {['cnic', 'name', 'fingerprint', 'weapon'].map((type) => (
            <button
              key={type}
              onClick={() => setSearchType(type)}
              className={`px-4 py-2 rounded-lg capitalize ${
                searchType === type
                  ? 'bg-crime-primary text-white'
                  : 'bg-gray-100 text-gray-700 hover:bg-gray-200'
              }`}
            >
              {type === 'cnic' ? 'CNIC' : type}
            </button>
          ))}
        </div>
        
        <form onSubmit={handleSearch} className="flex space-x-4">
          <div className="flex-1 relative">
            <MagnifyingGlassIcon className="absolute left-3 top-1/2 transform -translate-y-1/2 w-5 h-5 text-gray-400" />
            <input
              type="text"
              value={searchQuery}
              onChange={(e) => setSearchQuery(e.target.value)}
              placeholder={
                searchType === 'cnic' ? 'Enter CNIC (e.g., 42101-1234567-8)' :
                searchType === 'name' ? 'Enter criminal name' :
                searchType === 'fingerprint' ? 'Enter fingerprint ID' :
                'Enter weapon serial number'
              }
              className="w-full pl-10 pr-4 py-3 border border-gray-300 rounded-lg focus:ring-2 focus:ring-crime-primary focus:border-transparent"
            />
          </div>
          <button
            type="submit"
            disabled={loading}
            className="bg-crime-primary text-white px-6 py-3 rounded-lg hover:bg-blue-700 transition-colors disabled:opacity-50"
          >
            {loading ? 'Searching...' : 'Search'}
          </button>
        </form>
      </div>
      
      {/* Search Results */}
      {results.length > 0 && (
        <div className="mt-6">
          <h3 className="text-lg font-semibold text-gray-700 mb-4">Search Results</h3>
          <div className="space-y-4">
            {results.map((criminal) => (
              <div key={criminal.criminal_id} className="border border-gray-200 rounded-lg p-4 hover:bg-gray-50">
                <div className="flex items-start space-x-4">
                  <UserCircleIcon className="w-12 h-12 text-gray-400 flex-shrink-0" />
                  <div className="flex-1">
                    <div className="flex justify-between">
                      <h4 className="text-lg font-bold text-gray-900">{criminal.name}</h4>
                      <span className={`px-3 py-1 text-xs font-semibold rounded-full ${
                        criminal.status === 'Wanted' ? 'bg-red-100 text-red-800' :
                        criminal.status === 'Arrested' ? 'bg-yellow-100 text-yellow-800' :
                        'bg-green-100 text-green-800'
                      }`}>
                        {criminal.status}
                      </span>
                    </div>
                    <p className="text-sm text-gray-600 mt-1">CNIC: {criminal.cnic}</p>
                    <p className="text-sm text-gray-600">Age: {criminal.age} | {criminal.gender}</p>
                    <p className="text-sm text-gray-600 mt-2">{criminal.address}</p>
                    <div className="mt-3 flex space-x-3">
                      <button className="text-sm text-crime-primary hover:underline">
                        View Full Profile
                      </button>
                      <button className="text-sm text-green-600 hover:underline">
                        Edit Record
                      </button>
                      <button className="text-sm text-red-600 hover:underline">
                        Delete
                      </button>
                    </div>
                  </div>
                </div>
              </div>
            ))}
          </div>
        </div>
      )}
      
      {results.length === 0 && searchQuery && !loading && (
        <div className="text-center py-8 text-gray-500">
          No criminals found matching your search.
        </div>
      )}
    </div>
  );
};

export default CriminalSearch;
