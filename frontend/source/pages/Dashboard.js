import React, { useState, useEffect } from 'react';
import StatsCard from '../components/Dashboard/StatsCard';
import CriminalTable from '../components/Dashboard/CriminalTable';
import CriminalSearch from '../components/Search/CriminalSearch';
import {
  UserGroupIcon,
  DocumentReportIcon,
  ShieldCheckIcon,
  ChartBarIcon,
} from '@heroicons/react/24/outline';
import { statsAPI } from '../services/api';

const Dashboard = () => {
  const [stats, setStats] = useState({
    total_criminals: 0,
    wanted_criminals: 0,
    pending_reports: 0,
    processed_reports: 0,
    total_weapons: 0,
  });
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetchStats();
  }, []);

  const fetchStats = async () => {
    try {
      const response = await statsAPI.getSystemStats();
      if (response.data.success) {
        setStats(response.data.stats);
      }
    } catch (error) {
      console.error('Error fetching stats:', error);
    } finally {
      setLoading(false);
    }
  };

  const statCards = [
    {
      title: 'Total Criminals',
      value: stats.total_criminals,
      icon: UserGroupIcon,
      color: 'bg-blue-500',
      change: +12,
    },
    {
      title: 'Wanted Criminals',
      value: stats.wanted_criminals,
      icon: ShieldCheckIcon,
      color: 'bg-red-500',
      change: -5,
    },
    {
      title: 'Pending Reports',
      value: stats.pending_reports,
      icon: DocumentReportIcon,
      color: 'bg-yellow-500',
      change: +8,
    },
    {
      title: 'Weapons Registered',
      value: stats.total_weapons,
      icon: ChartBarIcon,
      color: 'bg-green-500',
      change: +3,
    },
  ];

  return (
    <div>
      <div className="mb-8">
        <h1 className="text-3xl font-bold text-gray-900">Crime Intelligence Dashboard</h1>
        <p className="text-gray-600 mt-2">Monitor criminal activities and manage law enforcement operations</p>
      </div>

      {/* Stats Grid */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-6 mb-8">
        {statCards.map((card, index) => (
          <StatsCard key={index} {...card} />
        ))}
      </div>

      <div className="grid grid-cols-1 lg:grid-cols-3 gap-8">
        <div className="lg:col-span-2">
          <CriminalTable />
        </div>
        <div>
          <CriminalSearch />
        </div>
      </div>

      {/* Recent Activity */}
      <div className="mt-8 bg-white rounded-xl shadow-sm p-6">
        <h2 className="text-xl font-bold text-gray-800 mb-4">Recent Activity</h2>
        <div className="space-y-4">
          {[
            { action: 'New criminal added', user: 'Officer Khan', time: '10 min ago', type: 'add' },
            { action: 'Report FIR-2024-001 processed', user: 'Inspector Ahmed', time: '25 min ago', type: 'process' },
            { action: 'Weapon registered', user: 'Constable Ali', time: '1 hour ago', type: 'register' },
            { action: 'Criminal profile updated', user: 'Supervisor Raza', time: '2 hours ago', type: 'update' },
          ].map((activity, index) => (
            <div key={index} className="flex items-center justify-between py-3 border-b border-gray-100 last:border-0">
              <div className="flex items-center space-x-3">
                <div className={`w-10 h-10 rounded-full flex items-center justify-center ${
                  activity.type === 'add' ? 'bg-blue-100' :
                  activity.type === 'process' ? 'bg-green-100' :
                  activity.type === 'register' ? 'bg-purple-100' :
                  'bg-yellow-100'
                }`}>
                  <span className={`text-sm font-bold ${
                    activity.type === 'add' ? 'text-blue-600' :
                    activity.type === 'process' ? 'text-green-600' :
                    activity.type === 'register' ? 'text-purple-600' :
                    'text-yellow-600'
                  }`}>
                    {activity.action.charAt(0)}
                  </span>
                </div>
                <div>
                  <p className="font-medium text-gray-900">{activity.action}</p>
                  <p className="text-sm text-gray-500">by {activity.user}</p>
                </div>
              </div>
              <span className="text-sm text-gray-500">{activity.time}</span>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default Dashboard;
