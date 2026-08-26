(function(){
  document.querySelectorAll('.idx-col').forEach(function(w){
    var c=w.querySelector('input[type=color]'), t=w.querySelector('input[type=text]');
    c.addEventListener('input',function(){t.value=c.value.slice(1);t.dispatchEvent(new Event('change',{bubbles:true}));});
    t.addEventListener('input',function(){if(/^[0-9a-fA-F]{6}$/.test(t.value))c.value='#'+t.value;});
  });
  var s=document.createElement('style');s.textContent='.idx-grid{display:grid;grid-template-columns:max-content 1fr;gap:10px 18px;max-width:720px;align-items:center}.idx-col{display:flex;gap:8px;align-items:center}.idx-col input[type=text]{width:90px}';document.head.appendChild(s);
})();
