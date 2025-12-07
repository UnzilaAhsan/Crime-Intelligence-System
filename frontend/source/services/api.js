import axios from 'axios';

const API_BASE_URL = process.env.REACT_APP_API_URL || 'http://localhost:8080';

const api = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'application/json',
  },
});

// Criminal endpoints
export const criminalAPI = {
  getAll: () => api.get('/api/criminals'),
  getByCNIC: (cnic) => api.get(`/api/criminals/${cnic}`),
  create: (criminalData) => api.post('/api/criminals', criminalData),
  update: (id, criminalData) => api.put(`/api/criminals/${id}`, criminalData),
  delete: (id) => api.delete(`/api/criminals/${id}`),
  search: (query) => api.get(`/api/criminals/search?q=${query}`),
};

// Report endpoints
export const reportAPI = {
  getAll: () => api.get('/api/reports'),
  getPending: () => api.get('/api/reports/pending'),
  create: (reportData) => api.post('/api/reports', reportData),
  process: (id) => api.put(`/api/reports/${id}/process`),
};

// Statistics endpoints
export const statsAPI = {
  getSystemStats: () => api.get('/api/stats'),
  getCrimePatterns: () => api.get('/api/stats/patterns'),
};

export default api;
